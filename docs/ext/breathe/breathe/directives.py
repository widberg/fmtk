from breathe.directive.base import create_warning
from breathe.directive.file import DoxygenFileDirective, AutoDoxygenFileDirective
from breathe.directive.index import DoxygenIndexDirective, AutoDoxygenIndexDirective
from breathe.exception import BreatheError
from breathe.finder.factory import FinderFactory
from breathe.directive.base import BaseDirective
from breathe.file_state_cache import MTimeError
from breathe.parser import DoxygenParserFactory, ParserError, FileIOError
from breathe.process import AutoDoxygenProcessHandle
from breathe.project import ProjectInfoFactory, ProjectError
from breathe.renderer import format_parser_error, RenderContext
from breathe.renderer.sphinxrenderer import WithContext
from breathe.renderer.filter import Filter
from breathe.renderer.mask import (
    MaskFactory, NullMaskFactory, NoParameterNamesMask
)
from breathe.renderer.sphinxrenderer import SphinxRenderer
from breathe.renderer.target import create_target_handler

from sphinx.application import Sphinx
from sphinx.domains import cpp

from docutils import nodes
from docutils.nodes import Node
from docutils.parsers.rst.directives import unchanged_required, unchanged, flag

import os
import re
import subprocess

from typing import Any, List, Optional, Type  # noqa


class NoMatchingFunctionError(BreatheError):
    pass


class UnableToResolveFunctionError(BreatheError):
    def __init__(self, signatures: List[str]) -> None:
        self.signatures = signatures


class DoxygenFunctionDirective(BaseDirective):
    required_arguments = 1
    option_spec = {
        "path": unchanged_required,
        "project": unchanged_required,
        "outline": flag,
        "no-link": flag,
    }
    has_content = False
    final_argument_whitespace = True

    def run(self) -> List[Node]:
        # Separate possible arguments (delimited by a "(") from the namespace::name
        match = re.match(r"([^(]*)(.*)", self.arguments[0])
        assert match is not None  # TODO: this is probably not appropriate, for now it fixes typing
        namespaced_function, args = match.group(1), match.group(2)

        # Split the namespace and the function name
        try:
            (namespace, function_name) = namespaced_function.rsplit("::", 1)
        except ValueError:
            (namespace, function_name) = "", namespaced_function
        namespace = namespace.strip()
        function_name = function_name.strip()

        try:
            project_info = self.project_info_factory.create_project_info(self.options)
        except ProjectError as e:
            warning = create_warning(None, self.state, self.lineno)
            return warning.warn('doxygenfunction: %s' % e)

        try:
            finder = self.finder_factory.create_finder(project_info)
        except MTimeError as e:
            warning = create_warning(None, self.state, self.lineno)
            return warning.warn('doxygenfunction: %s' % e)

        # Extract arguments from the function name.
        args = self._parse_args(args)

        finder_filter = self.filter_factory.create_function_and_all_friend_finder_filter(
            namespace, function_name)

        # TODO: find a more specific type for the Doxygen nodes
        matchesAll = []  # type: List[Any]
        finder.filter_(finder_filter, matchesAll)
        matches = []
        for m in matchesAll:
            # only take functions and friend functions
            # ignore friend classes
            node = m[0]
            if node.kind == 'friend' and not node.argsstring:
                continue
            matches.append(m)

        # Create it ahead of time as it is cheap and it is ugly to declare it for both exception
        # clauses below
        warning = create_warning(
            project_info,
            self.state,
            self.lineno,
            namespace='%s::' % namespace if namespace else '',
            function=function_name,
            args=str(args)
            )

        try:
            node_stack = self._resolve_function(matches, args, project_info)
        except NoMatchingFunctionError:
            return warning.warn('doxygenfunction: Cannot find function "{namespace}{function}" '
                                '{tail}')
        except UnableToResolveFunctionError as error:
            message = 'doxygenfunction: Unable to resolve function ' \
                '"{namespace}{function}" with arguments {args} {tail}.\n' \
                'Potential matches:\n'

            text = ''
            for i, entry in enumerate(sorted(error.signatures)):
                text += '- %s\n' % entry
            block = nodes.literal_block('', '', nodes.Text(text))
            formatted_message = warning.format(message)
            warning_nodes = [
                nodes.paragraph("", "", nodes.Text(formatted_message)),
                block
            ]
            result = warning.warn(message, rendered_nodes=warning_nodes,
                                  unformatted_suffix=text)
            return result

        target_handler = create_target_handler(self.options, project_info, self.state.document)
        filter_ = self.filter_factory.create_outline_filter(self.options)

        return self.render(node_stack, project_info, filter_, target_handler, NullMaskFactory(),
                           self.directive_args)

    def _parse_args(self, function_description: str) -> Optional[cpp.ASTParametersQualifiers]:
        if function_description == '':
            return None

        parser = cpp.DefinitionParser(function_description,
                                      location=self.get_source_info(),
                                      config=self.config)
        paramQual = parser._parse_parameters_and_qualifiers(paramMode='function')
        # now erase the parameter names
        for p in paramQual.args:
            if p.arg is None:
                assert p.ellipsis
                continue
            declarator = p.arg.type.decl
            while hasattr(declarator, 'next'):
                declarator = declarator.next  # type: ignore
            assert hasattr(declarator, 'declId')
            declarator.declId = None  # type: ignore
            p.arg.init = None  # type: ignore
        return paramQual

    def _create_function_signature(self, node_stack, project_info, filter_, target_handler,
                                   mask_factory, directive_args) -> str:
        "Standard render process used by subclasses"

        try:
            object_renderer = SphinxRenderer(
                self.parser_factory.app,
                project_info,
                node_stack,
                self.state,
                self.state.document,
                target_handler,
                self.parser_factory.create_compound_parser(project_info),
                filter_,
            )
        except ParserError as e:
            return format_parser_error("doxygenclass", e.error, e.filename, self.state,
                                       self.lineno, True)
        except FileIOError as e:
            return format_parser_error("doxygenclass", e.error, e.filename, self.state,
                                       self.lineno, False)

        context = RenderContext(node_stack, mask_factory, directive_args)
        node = node_stack[0]
        with WithContext(object_renderer, context):
            # this part should be kept in sync with visit_function in sphinxrenderer
            name = node.get_name()
            # assume we are only doing this for C++ declarations
            declaration = ' '.join([
                object_renderer.create_template_prefix(node),
                ''.join(n.astext() for n in object_renderer.render(node.get_type())),
                name,
                node.get_argsstring()
            ])
        parser = cpp.DefinitionParser(declaration,
                                      location=self.get_source_info(),
                                      config=self.config)
        ast = parser.parse_declaration('function', 'function')
        return str(ast)

    def _resolve_function(self, matches, args: Optional[cpp.ASTParametersQualifiers], project_info):
        if not matches:
            raise NoMatchingFunctionError()

        res = []
        candSignatures = []
        for entry in matches:
            text_options = {'no-link': u'', 'outline': u''}

            # Render the matches to docutils nodes
            target_handler = create_target_handler({'no-link': u''},
                                                   project_info, self.state.document)
            filter_ = self.filter_factory.create_outline_filter(text_options)
            mask_factory = MaskFactory({'param': NoParameterNamesMask})

            # Override the directive args for this render
            directive_args = self.directive_args[:]
            directive_args[2] = text_options

            signature = self._create_function_signature(entry, project_info, filter_,
                                                        target_handler,
                                                        mask_factory, directive_args)
            candSignatures.append(signature)

            if args is not None:
                match = re.match(r"([^(]*)(.*)", signature)
                assert match
                _match_args = match.group(2)

                # Parse the text to find the arguments
                match_args = self._parse_args(_match_args)

                # Match them against the arg spec
                if args != match_args:
                    continue

            res.append((entry, signature))

        if len(res) == 1:
            return res[0][0]
        else:
            raise UnableToResolveFunctionError(candSignatures)


class _DoxygenClassLikeDirective(BaseDirective):
    required_arguments = 1
    optional_arguments = 0
    final_argument_whitespace = True
    option_spec = {
        "path": unchanged_required,
        "project": unchanged_required,
        "members": unchanged,
        "membergroups": unchanged_required,
        "members-only": flag,
        "protected-members": flag,
        "private-members": flag,
        "undoc-members": flag,
        "show": unchanged_required,
        "outline": flag,
        "no-link": flag,
    }
    has_content = False

    def run(self) -> List[Node]:
        name = self.arguments[0]

        try:
            project_info = self.project_info_factory.create_project_info(self.options)
        except ProjectError as e:
            warning = create_warning(None, self.state, self.lineno, kind=self.kind)
            return warning.warn('doxygen{kind}: %s' % e)

        try:
            finder = self.finder_factory.create_finder(project_info)
        except MTimeError as e:
            warning = create_warning(None, self.state, self.lineno, kind=self.kind)
            return warning.warn('doxygen{kind}: %s' % e)

        finder_filter = self.filter_factory.create_compound_finder_filter(name, self.kind)

        # TODO: find a more specific type for the Doxygen nodes
        matches = []  # type: List[Any]
        finder.filter_(finder_filter, matches)

        if len(matches) == 0:
            warning = create_warning(project_info, self.state, self.lineno, name=name,
                                     kind=self.kind)
            return warning.warn('doxygen{kind}: Cannot find class "{name}" {tail}')

        target_handler = create_target_handler(self.options, project_info, self.state.document)
        filter_ = self.filter_factory.create_class_filter(name, self.options)

        mask_factory = NullMaskFactory()
        return self.render(matches[0], project_info, filter_, target_handler, mask_factory,
                           self.directive_args)


class DoxygenClassDirective(_DoxygenClassLikeDirective):
    kind = "class"


class DoxygenStructDirective(_DoxygenClassLikeDirective):
    kind = "struct"


class DoxygenInterfaceDirective(_DoxygenClassLikeDirective):
    kind = "interface"


class _DoxygenContentBlockDirective(BaseDirective):
    """Base class for namespace and group directives which have very similar behaviours"""

    required_arguments = 1
    optional_arguments = 1
    option_spec = {
        "path": unchanged_required,
        "project": unchanged_required,
        "content-only": flag,
        "outline": flag,
        "members": flag,
        "protected-members": flag,
        "private-members": flag,
        "undoc-members": flag,
        "no-link": flag
    }
    has_content = False

    def run(self) -> List[Node]:
        name = self.arguments[0]

        try:
            project_info = self.project_info_factory.create_project_info(self.options)
        except ProjectError as e:
            warning = create_warning(None, self.state, self.lineno, kind=self.kind)
            return warning.warn('doxygen{kind}: %s' % e)

        try:
            finder = self.finder_factory.create_finder(project_info)
        except MTimeError as e:
            warning = create_warning(None, self.state, self.lineno, kind=self.kind)
            return warning.warn('doxygen{kind}: %s' % e)

        finder_filter = self.filter_factory.create_finder_filter(self.kind, name)

        # TODO: find a more specific type for the Doxygen nodes
        matches = []  # type: List[Any]
        finder.filter_(finder_filter, matches)

        # It shouldn't be possible to have too many matches as namespaces & groups in their nature
        # are merged together if there are multiple declarations, so we only check for no matches
        if not matches:
            warning = create_warning(project_info, self.state, self.lineno, name=name,
                                     kind=self.kind)
            return warning.warn('doxygen{kind}: Cannot find {kind} "{name}" {tail}')

        if 'content-only' in self.options and self.kind != "page":
            # Unpack the single entry in the matches list
            (node_stack,) = matches

            filter_ = self.filter_factory.create_content_filter(self.kind, self.options)
            # Having found the compound node for the namespace or group in the index we want to grab
            # the contents of it which match the filter
            contents_finder = self.finder_factory.create_finder_from_root(node_stack[0],
                                                                          project_info)
            # TODO: find a more specific type for the Doxygen nodes
            contents = []  # type: List[Any]
            contents_finder.filter_(filter_, contents)

            # Replaces matches with our new starting points
            matches = contents

        target_handler = create_target_handler(self.options, project_info, self.state.document)
        filter_ = self.filter_factory.create_render_filter(self.kind, self.options)

        node_list = []
        for node_stack in matches:
            object_renderer = SphinxRenderer(
                self.parser_factory.app,
                project_info,
                node_stack,
                self.state,
                self.state.document,
                target_handler,
                self.parser_factory.create_compound_parser(project_info),
                filter_
            )

            mask_factory = NullMaskFactory()
            context = RenderContext(node_stack, mask_factory, self.directive_args)
            node_list.extend(object_renderer.render(context.node_stack[0], context))

        return node_list


class DoxygenNamespaceDirective(_DoxygenContentBlockDirective):
    kind = "namespace"


class DoxygenGroupDirective(_DoxygenContentBlockDirective):
    kind = "group"
    option_spec = {
        **_DoxygenContentBlockDirective.option_spec,
        "inner": flag,
    }


class DoxygenPageDirective(_DoxygenContentBlockDirective):
    kind = "page"
    option_spec = {
        "path": unchanged_required,
        "project": unchanged_required,
        "content-only": flag,
    }


# TODO: is this comment still relevant?
# This class was the same as the DoxygenBaseDirective above, except that it
# wraps the output in a definition_list before passing it back. This should be
# abstracted in a far nicer way to avoid repeating so much code
#
# Now we've removed the definition_list wrap so we really need to refactor this!
class _DoxygenBaseItemDirective(BaseDirective):
    required_arguments = 1
    optional_arguments = 1
    option_spec = {
        "path": unchanged_required,
        "project": unchanged_required,
        "outline": flag,
        "no-link": flag,
        }
    has_content = False

    def create_finder_filter(self, namespace: str, name: str) -> Filter:
        """Creates a filter to find the node corresponding to this item."""

        return self.filter_factory.create_member_finder_filter(
            namespace, name, self.kind)

    def run(self) -> List[Node]:
        try:
            namespace, name = self.arguments[0].rsplit("::", 1)
        except ValueError:
            namespace, name = "", self.arguments[0]

        try:
            project_info = self.project_info_factory.create_project_info(self.options)
        except ProjectError as e:
            warning = create_warning(None, self.state, self.lineno, kind=self.kind)
            return warning.warn('doxygen{kind}: %s' % e)

        try:
            finder = self.finder_factory.create_finder(project_info)
        except MTimeError as e:
            warning = create_warning(None, self.state, self.lineno, kind=self.kind)
            return warning.warn('doxygen{kind}: %s' % e)

        finder_filter = self.create_finder_filter(namespace, name)

        # TODO: find a more specific type for the Doxygen nodes
        matches = []  # type: List[Any]
        finder.filter_(finder_filter, matches)

        if len(matches) == 0:
            display_name = "%s::%s" % (namespace, name) if namespace else name
            warning = create_warning(project_info, self.state, self.lineno, kind=self.kind,
                                     display_name=display_name)
            return warning.warn('doxygen{kind}: Cannot find {kind} "{display_name}" {tail}')

        target_handler = create_target_handler(self.options, project_info, self.state.document)
        filter_ = self.filter_factory.create_outline_filter(self.options)

        node_stack = matches[0]
        mask_factory = NullMaskFactory()
        return self.render(node_stack, project_info, filter_, target_handler, mask_factory,
                           self.directive_args)


class DoxygenVariableDirective(_DoxygenBaseItemDirective):
    kind = "variable"


class DoxygenDefineDirective(_DoxygenBaseItemDirective):
    kind = "define"


class DoxygenEnumDirective(_DoxygenBaseItemDirective):
    kind = "enum"


class DoxygenEnumValueDirective(_DoxygenBaseItemDirective):
    kind = "enumvalue"

    def create_finder_filter(self, namespace: str, name: str) -> Filter:
        return self.filter_factory.create_enumvalue_finder_filter(name)


class DoxygenTypedefDirective(_DoxygenBaseItemDirective):
    kind = "typedef"


class DoxygenUnionDirective(_DoxygenBaseItemDirective):
    kind = "union"

    def create_finder_filter(self, namespace: str, name: str) -> Filter:
        # Unions are stored in the xml file with their fully namespaced name
        # We're using C++ namespaces here, it might be best to make this file
        # type dependent
        #
        xml_name = "%s::%s" % (namespace, name) if namespace else name
        return self.filter_factory.create_compound_finder_filter(xml_name, 'union')


# Setup Administration
# --------------------

class DirectiveContainer:
    def __init__(self, app: Sphinx, directive: Type[BaseDirective],
                 finder_factory: FinderFactory, project_info_factory: ProjectInfoFactory,
                 parser_factory: DoxygenParserFactory):
        self.app = app
        self.directive = directive
        self.finder_factory = finder_factory
        self.project_info_factory = project_info_factory
        self.parser_factory = parser_factory

        # Required for sphinx to inspect
        self.required_arguments = directive.required_arguments
        self.optional_arguments = directive.optional_arguments
        self.option_spec = directive.option_spec
        self.has_content = directive.has_content
        self.final_argument_whitespace = directive.final_argument_whitespace

    def __call__(self, *args):
        call_args = [
            self.finder_factory,
            self.project_info_factory,
            self.parser_factory
        ]
        call_args.extend(args)
        return self.directive(*call_args)


def setup(app: Sphinx) -> None:
    directives = {
        "doxygenindex": DoxygenIndexDirective,
        "autodoxygenindex": AutoDoxygenIndexDirective,
        "doxygenfunction": DoxygenFunctionDirective,
        "doxygenstruct": DoxygenStructDirective,
        "doxygenclass": DoxygenClassDirective,
        "doxygeninterface": DoxygenInterfaceDirective,
        "doxygenvariable": DoxygenVariableDirective,
        "doxygendefine": DoxygenDefineDirective,
        "doxygenenum": DoxygenEnumDirective,
        "doxygenenumvalue": DoxygenEnumValueDirective,
        "doxygentypedef": DoxygenTypedefDirective,
        "doxygenunion": DoxygenUnionDirective,
        "doxygennamespace": DoxygenNamespaceDirective,
        "doxygengroup": DoxygenGroupDirective,
        "doxygenfile": DoxygenFileDirective,
        "autodoxygenfile": AutoDoxygenFileDirective,
        "doxygenpage": DoxygenPageDirective,
    }

    # note: the parser factory contains a cache of the parsed XML
    # note: the project_info_factory also contains some caching stuff
    # TODO: is that actually safe for when reading in parallel?
    project_info_factory = ProjectInfoFactory(app)
    parser_factory = DoxygenParserFactory(app)
    finder_factory = FinderFactory(app, parser_factory)
    for name, directive in directives.items():
        # ordinarily app.add_directive takes a class it self, but we need to inject extra arguments
        # so we give a DirectiveContainer object which has an overloaded __call__ operator.
        app.add_directive(name, DirectiveContainer(  # type: ignore
            app,
            directive,
            finder_factory,
            project_info_factory,
            parser_factory
            ))

    app.add_config_value("breathe_projects", {}, True)  # Dict[str, str]
    app.add_config_value("breathe_default_project", "", True)  # str
    # Provide reasonable defaults for domain_by_extension mapping. Can be overridden by users.
    app.add_config_value("breathe_domain_by_extension",
                         {'py': 'py', 'cs': 'cs'}, True)  # Dict[str, str]
    app.add_config_value("breathe_domain_by_file_pattern", {}, True)  # Dict[str, str]
    app.add_config_value("breathe_projects_source", {}, True)
    app.add_config_value("breathe_build_directory", '', True)
    app.add_config_value("breathe_default_members", (), True)
    app.add_config_value("breathe_show_define_initializer", False, 'env')
    app.add_config_value("breathe_show_enumvalue_initializer", False, 'env')
    app.add_config_value("breathe_implementation_filename_extensions", ['.c', '.cc', '.cpp'], True)
    app.add_config_value("breathe_doxygen_config_options", {}, True)
    app.add_config_value("breathe_use_project_refids", False, "env")
    app.add_config_value("breathe_order_parameters_first", False, 'env')
    app.add_config_value("breathe_separate_member_pages", False, 'env')

    breathe_css = "breathe.css"
    if (os.path.exists(os.path.join(app.confdir, "_static", breathe_css))):  # type: ignore
        app.add_css_file(breathe_css)

    def write_file(directory, filename, content):
        # Check the directory exists
        if not os.path.exists(directory):
            os.makedirs(directory)

        # Write the file with the provided contents
        with open(os.path.join(directory, filename), "w") as f:
            f.write(content)

    doxygen_handle = AutoDoxygenProcessHandle(
        subprocess.check_call,
        write_file,
        project_info_factory)

    def doxygen_hook(app):
        doxygen_handle.generate_xml(
            app.config.breathe_projects_source,
            app.config.breathe_doxygen_config_options
        )
    app.connect("builder-inited", doxygen_hook)
