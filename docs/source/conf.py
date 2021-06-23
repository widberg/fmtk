# -*- coding: utf-8 -*-
#
# Configuration file for the Sphinx documentation builder.
#
# This file does only contain a selection of the most common options. For a
# full list see the documentation:
# http://www.sphinx-doc.org/en/master/config

# -- Imports -----------------------------------------------------------------

import os, subprocess, sys, re

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))

script_dir = os.path.dirname(os.path.abspath(__file__))

def abspath(relpath):
    return os.path.abspath(os.path.join(script_dir, relpath))

sys.path.append(abspath('../ext/breathe/'))

# -- Project information -----------------------------------------------------

project = 'FMTK'
copyright = '2021, widberg'
author = 'widberg'

with open('../../CMakeLists.txt') as f:
    for line in f.readlines():
        m = re.match(r'^project\(fmtk.+VERSION\s+(\d+\.\d+\.\d+).+\)$', line)
        if m:
            version = m.group(1)
            break

release = version

# -- General configuration ---------------------------------------------------

# If your documentation needs a minimal Sphinx version, state it here.
#
# needs_sphinx = '1.0'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'breathe',
    'sphinxcontrib.luadomain',
    'sphinx_lua'
]

lua_source_path = ["../../fmtklua/fmtk.lua"]

breathe_projects = {
   'fmtk': abspath('../build/doxygen/xml/')
}

for breathe_project in breathe_projects:
    if not os.path.isdir(breathe_projects[breathe_project]):
        os.makedirs(breathe_projects[breathe_project])

breathe_default_project = 'fmtk'

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# The suffix(es) of source filenames.
# You can specify multiple suffix as a list of string:
#
# source_suffix = ['.rst', '.md']
source_suffix = '.rst'

# The master toctree document.
master_doc = 'index'

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = None

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = None


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme'

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
#
html_theme_options = {
    'collapse_navigation': False,
    'style_nav_header_background': '#1e5696'
}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# Custom sidebar templates, must be a dictionary that maps document names
# to template names.
#
# The default sidebars (for documents that don't match any pattern) are
# defined by theme itself.  Builtin themes are using these templates by
# default: ``['localtoc.html', 'relations.html', 'sourcelink.html',
# 'searchbox.html']``.
#
# html_sidebars = {}

html_logo = abspath('img/logo.png')


# -- Options for HTMLHelp output ---------------------------------------------

# Output file base name for HTML help builder.
htmlhelp_basename = 'FMTKdoc'


# -- Options for LaTeX output ------------------------------------------------

latex_elements = {
    # The paper size ('letterpaper' or 'a4paper').
    #
    # 'papersize': 'letterpaper',

    # The font size ('10pt', '11pt' or '12pt').
    #
    # 'pointsize': '10pt',

    # Additional stuff for the LaTeX preamble.
    #
    # 'preamble': '',

    # Latex figure (float) alignment
    #
    # 'figure_align': 'htbp',
}

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [
    (master_doc, 'FMTK.tex', 'FMTK Documentation',
     'widberg', 'manual'),
]


# -- Options for manual page output ------------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [
    (master_doc, 'fmtk', 'FMTK Documentation',
     [author], 1)
]


# -- Options for Texinfo output ----------------------------------------------

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
texinfo_documents = [
    (master_doc, 'FMTK', 'FMTK Documentation',
     author, 'FMTK', 'FMTK is an open source game engine written in C++.',
     'Miscellaneous'),
]


# -- Options for Epub output -------------------------------------------------

# Bibliographic Dublin Core info.
epub_title = project

# The unique identifier of the text. This can be a ISBN number
# or the project homepage.
#
# epub_identifier = ''

# A unique identification for the text.
#
# epub_uid = ''

# A list of files that should not be packed into the epub file.
epub_exclude_files = ['search.html']

# -- Doxygen -----------------------------------------------------------------

def run_doxygen(folder):
    """Run the doxygen command in the designated folder"""

    folder = abspath(folder)

    try:
        print('running doxygen in %s' % folder)
        retcode = subprocess.call('doxygen', cwd=folder)
        if retcode < 0:
            sys.stderr.write('doxygen terminated by signal %s' % (-retcode))
    except OSError as e:
        sys.stderr.write('doxygen execution failed: %s' % e)


def builder_inited(app):
    """Run the doxygen command"""
    run_doxygen('../')


def setup(app):

    # Add hook for building doxygen when needed
    app.connect("builder-inited", builder_inited)

    app.add_css_file('style.css')
