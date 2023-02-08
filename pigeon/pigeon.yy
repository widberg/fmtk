%no-lines
%require "3.4.1"
%language "c++"

%skeleton "lalr1.cc"

%define api.token.constructor
%define api.value.type variant
%define api.location.file none
%define parse.assert true
%define parse.error verbose
%define parse.trace true
/* %define api.value.automove true */

%define api.namespace {pigeon}
%define api.parser.class {Parser}

%param {Context *ctx}

%locations

%token END_OF_FILE 0

%token IDENTIFIER TYPE
%token ENUM "enum"
%token USE "use"
%token AS "as"
%token STRUCT "struct"
%token DEPRICATED "depricated"
%token NAMESPACE "namespace"

%type<std::string> IDENTIFIER
%type<std::shared_ptr<Type>> TYPE type
%type<std::vector<std::string>> enum_body
%type<std::vector<Declaration>> declaration_list

%code requires
{
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <optional>
#include <string>
#include <cstring>
#include <CLI/CLI.hpp>

// Bison generates weird switch statements
#ifdef _MSC_VER
#pragma warning( disable : 4065 )
#endif
}

%code top
{
    namespace pigeon {
        class Context;
        class Type;
        struct Declaration;
    }
}

%code
{
#define TOKEN(name) do { return pigeon::Parser::make_##name(ctx->loc); } while(0)
#define TOKENV(name, ...) do { return pigeon::Parser::make_##name(__VA_ARGS__, ctx->loc); } while(0)
#define PIGEON_ASSERT(cond, loc, msg) do { if (!cond) { pigeon::Parser::error(loc, msg); YYERROR; } } while(0)


namespace pigeon
{
    Parser::symbol_type yylex(Context *context);

    class DumpC
    {
        virtual void dump_c(std::ostream& out) const = 0;
    };

    class CType
    {
    public:
        virtual std::string declaration(std::string const& variable_name) const = 0;
        virtual std::string type_definition(std::string const& type_name) const = 0;
        virtual bool can_return() const = 0;
        virtual bool by_reference() const = 0;

    };

    class Type : public CType
    {
    public:
        virtual std::string declaration(std::string const& variable_name) const override = 0;
        virtual std::string type_definition(std::string const& type_name) const = 0;
        virtual bool can_return() const override = 0;
        virtual bool by_reference() const override = 0;
        virtual ~Type() {}
    };
    
    class PrimitiveType : public Type
    {
    public:
        enum class Kind
        {
            U8,
            U16,
            U32,
            U64,
            I8,
            I16,
            I32,
            I64,
            F32,
            F64,
            BOOL,
            HANDLE,
            STRING,
        };

        PrimitiveType(Kind kind)
            : kind(kind) {}
        
        virtual std::string declaration(std::string const& variable_name) const override
        {
            std::string declaration;
            switch (kind)
            {
            case Kind::U8:
                declaration = "uint8_t ";
                break;
            case Kind::U16:
                declaration = "uint16_t ";
                break;
            case Kind::U32:
                declaration = "uint32_t ";
                break;
            case Kind::U64:
                declaration = "uint64_t ";
                break;
            case Kind::I8:
                declaration = "int8_t ";
                break;
            case Kind::I16:
                declaration = "int16_t ";
                break;
            case Kind::I32:
                declaration = "int32_t ";
                break;
            case Kind::I64:
                declaration = "int64_t ";
                break;
            case Kind::F32:
                declaration = "float ";
                break;
            case Kind::F64:
                declaration = "double ";
                break;
            case Kind::BOOL:
                declaration = "bool ";
                break;
            case Kind::HANDLE:
                declaration = "void *";
                break;
            case Kind::STRING:
                declaration = "char const *";
                break;
            }

            return declaration + variable_name;
        }

        std::string type_definition(std::string const& type_name) const
        {
            /* return "typedef " + declaration(type_name) + ";"; */
            return "";
        }

        bool can_return() const override
        {
            return true;
        }
        bool by_reference() const override
        {
            return false;
        }
    private:
        Kind kind;
    };

    class TypeDefType : public Type
    {
    public:
        TypeDefType(std::shared_ptr<Type> type, std::string const& name)
            : type(type), name(name) {}
        
        virtual std::string declaration(std::string const& variable_name) const override
        {
            return type->declaration(variable_name);
        }

        std::string type_definition(std::string const& type_name) const
        {
            return "typedef " + name + " " + type_name + ";\n";
        }

        bool can_return() const override
        {
            return type->can_return();
        }
        bool by_reference() const override
        {
            return type->by_reference();
        }
    private:
        std::shared_ptr<Type> type;
        std::string name;
    };

    class OptionalType : public Type
    {
    public:
        OptionalType(std::shared_ptr<Type> type)
            : type(type) {}
        
        virtual std::string declaration(std::string const& variable_name) const override
        {
            return type->declaration("*" + variable_name);
        }

        std::string type_definition(std::string const& type_name) const
        {
            return "typedef " + declaration(type_name) + ";\n";
        }

        bool can_return() const override
        {
            return false;
        }
        bool by_reference() const override
        {
            return type->by_reference();
        }
    private:
        std::shared_ptr<Type> type;
    };

    class ArrayType : public Type
    {
    public:
        ArrayType(std::shared_ptr<Type> type)
            : type(type) {}
        
        virtual std::string declaration(std::string const& variable_name) const override
        {
            return type->declaration(variable_name + "[]");
        }

        std::string type_definition(std::string const& type_name) const override
        {
            return "typedef " + declaration(type_name) + ";\n";
        }

        bool can_return() const override
        {
            return false;
        }
        bool by_reference() const override
        {
            return true;
        }
    private:
        std::shared_ptr<Type> type;
    };
    
    class EnumType : public Type
    {
    public:
        EnumType(std::string const& name, std::vector<std::string> const& values)
            : name(name), values(values) {}
        
        virtual std::string declaration(std::string const& variable_name) const override
        {
            return name + "_t " + variable_name;
        }

        std::string type_definition(std::string const& type_name) const
        {
            std::string out;

            out += "typedef enum " + type_name + "\n{\n";

            for (std::string value : values)
            {
                out += value + ",\n";
            }

            out += "} " + type_name + "_t;\n";
            return out;
        }

        bool can_return() const override
        {
            return true;
        }
        bool by_reference() const override
        {
            return false;
        }
    private:
        std::string name;
        std::vector<std::string> values;
    };

    struct Declaration
    {
        std::string name;
        std::shared_ptr<Type> type;
    };

    class StructType : public Type
    {
    public:
        StructType(std::string const& name, std::vector<Declaration> const& members)
            : name(name), members(members) {}
        
        virtual std::string declaration(std::string const& variable_name) const override
        {
            return name + "_t *" + variable_name;
        }

        std::string type_definition(std::string const& type_name) const
        {
            std::string out;

            out += "typedef struct " + type_name + "\n{\n";

            for (Declaration member : members)
            {
                out += member.type->declaration(member.name) + ";\n";
            }

            out += "} " + type_name + "_t;\n";
            return out;
        }

        bool can_return() const override
        {
            return false;
        }
        bool by_reference() const override
        {
            return true;
        }
    private:
        std::string name;
        std::vector<Declaration> members;
    };

    class FunctionType : public Type
    {
    public:
        FunctionType(std::vector<Declaration> const& parameters, std::vector<Declaration> const& returns)
            : parameters(parameters), returns(returns) {}
        
        virtual std::string declaration(std::string const& variable_name) const override
        {
            // TODO: Generate something that makes sense here in regards to multiple return values
            std::string out;

            if (returns.size() == 0 || (!returns[0].type->can_return() && returns.size() == 1))
            {
                out += "void ";
            } else {
                out += returns[0].type->declaration("");
            }
            out += "(__stdcall *" + variable_name + ")(";
            
            bool has_parameters = false;

            for (auto parameter : parameters)
            {
                out += parameter.type->declaration(parameter.name) + ", ";

                has_parameters = true;
            }

            if (returns.size() > 0 && (!returns[0].type->can_return() || returns.size() > 1))
            {
                for (std::size_t i = (int)returns[0].type->can_return(); i < returns.size(); ++i)
                {
                    if (!returns[i].type->by_reference())
                    {
                        out += returns[i].type->declaration("*" + returns[i].name) + ", ";
                    }
                    else
                    {
                        out += returns[i].type->declaration(returns[i].name) + ", ";
                    }
                }

                has_parameters = true;
            }

            if (has_parameters)
            {
                out.pop_back();
                out.pop_back();
            }
            else
            {
                out += "void";
            }

            out += ")";

            return out;
        }

        std::string type_definition(std::string const& type_name) const
        {
            return "typedef " + declaration(type_name) + ";\n";
        }

        bool can_return() const override
        {
            return true;
        }
        bool by_reference() const override
        {
            return true;
        }
    private:
        std::vector<Declaration> parameters;
        std::vector<Declaration> returns;
    };

    static std::string to_upper_str(std::string str)
    {
        for (char& c : str)
        {
            c = std::toupper(c);
        }
        return str;
    }
    
    class Namespace : public DumpC
    {
    public:
        Namespace(std::string const& name, Namespace *parent)
            : name(name), uppercase_name(to_upper_str(name)), parent(parent) {}
        Namespace(std::string const& name)
            : name(name), uppercase_name(to_upper_str(name)), parent(nullptr) {}
        
        std::optional<std::shared_ptr<Type>> get_type(std::string_view name) const {
            for (auto it : types) {
                if (it.first == name)
                {
                    return it.second;
                }
            }

            if (parent)
            {
                if (auto type = parent->get_type(name))
                {
                    return type;
                }
            }

            return {};
        }

        void add_type(std::string const& name, std::shared_ptr<Type> type)
        {
            types.push_back({name, type});
        }

        void add_export(std::string const& name, std::shared_ptr<Type> type)
        {
            exports.push_back({name, type});
        }

        Namespace *get_child(std::string const& name)
        {
            for (auto child : children)
            {
                if (name == child->get_name())
                {
                    return child;
                }
            }

            return nullptr;
        }

        Namespace *get_or_add_namespace(std::string const& name)
        {
            if (auto child = get_child(name))
            {
                return child;
            }
            children.emplace_back(new Namespace(name, this));
            return children.back();
        }

        Namespace *get_parent()
        {
            return parent;
        }

        std::string const& get_name() const
        {
            return name;
        }

        std::string const& get_uppercase_name() const
        {
            return uppercase_name;
        }

        void dump(std::ostream& out) const {
            std::string prefix;
            for (Namespace *p = parent; p; p = p->get_parent())
            {
                prefix = p->get_name() + "_" + prefix;
            }

            for (auto type : types)
            {
                std::cout << prefix << type.first << '\n';
            }

            for (auto ekport : exports)
            {
                std::cout << prefix << ekport.first << '\n';
            }

            for (auto child : children)
            {
                child->dump(out);
            }
        }

        ~Namespace()
        {
            for (auto child : children)
            {
                delete child;
            }
        }

        std::vector<Namespace*> const& get_children() const
        {
            return children;
        }

        std::vector<std::pair<std::string, std::shared_ptr<Type>>> const& get_types() const
        {
            return types;
        }

        void dump_c(std::ostream& out) const
        {
            if (parent)
            {
                out << "struct" << "\n{\n";

            }
            else
            {
                out << "typedef struct " << name << "_api\n{\n";
            }
        
            for (auto type : types)
            {
                out << type.second->type_definition(type.first);
            }

            for (auto eksport : exports)
            {
                out << eksport.second->declaration(eksport.first) << ";\n";
            }

            for (Namespace *child : children)
            {
                child->dump_c(out);
                out << "\n";
            }

            if (parent)
            {
                out << "} *" << name << ";\n";
            }
            else
            {
                out << "} " << name << "_api_t;\n";
            }
        }

    private:
        std::string name;
        std::string uppercase_name;
        std::vector<Namespace*> children;
        Namespace *parent;
        std::vector<std::pair<std::string, std::shared_ptr<Type>>> types;
        std::vector<std::pair<std::string, std::shared_ptr<Type>>> exports;
    };

    class Context : public DumpC
    {
    public:
        Context(std::string const& name)
            : global(name), current_namespace(&global)
        {
            global.add_type("u8", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::U8)));
            global.add_type("u16", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::U16)));
            global.add_type("u32", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::U32)));
            global.add_type("u64", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::U64)));
            global.add_type("i8", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::I8)));
            global.add_type("i16", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::I16)));
            global.add_type("i32", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::I32)));
            global.add_type("i64", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::I64)));
            global.add_type("f32", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::F32)));
            global.add_type("f64", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::F64)));
            global.add_type("bool", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::BOOL)));
            global.add_type("handle", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::HANDLE)));
            global.add_type("string", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::STRING)));
        }
        bool interpret(char *input, unsigned int size, std::string input_filename, bool debug = false);
        bool interpret(std::istream& input_stream, std::string input_filename, bool debug);
        char *cur;
        char *mar;
        char *lim;
        location loc;
        Namespace global;
        Namespace *current_namespace;

        void dump_c(std::ostream& out) const
        {
            out << "#ifndef " << global.get_uppercase_name() << "_H\n";
            out << "#define " << global.get_uppercase_name() << "_H\n";
            out << "#ifdef __cplusplus\n";
            out << "extern \"C\"\n";
            out << "{\n";
            out << "#include <cstdint>\n";
            out << "#include <cstdbool>\n";
            out << "#define " << global.get_uppercase_name() << "API extern \"C\" __declspec(dllexport) __declspec(noinline) __stdcall\n";
            out << "#else // __cplusplus\n";
            out << "#include <stdint.h>\n";
            out << "#include <stdbool.h>\n";
            out << "#define " << global.get_uppercase_name() << "API __declspec(dllexport) __declspec(noinline) __stdcall\n";
            out << "#endif // __cplusplus\n\n";

            global.dump_c(out);

            out << "\n#ifdef __cplusplus\n";
            out << "}\n";
            out << "#endif // __cplusplus\n";
            out << "#endif // !" << global.get_uppercase_name() << "_H\n";
        }
    };

}


}

%start slist

%%

slist
    : slist stmt
    | %empty
    ;

stmt
    : "enum" IDENTIFIER docspec '{' enum_body '}' ';' { ctx->current_namespace->add_type($2, std::shared_ptr<Type>((Type*)new EnumType($2, $5)));}
    | "struct" IDENTIFIER docspec '{' declaration_list '}' ';' { ctx->current_namespace->add_type($2, std::shared_ptr<Type>((Type*)new StructType($2, $5)));}
    | "use" type "as" IDENTIFIER docspec ';' {ctx->current_namespace->add_type($4, std::shared_ptr<Type>((Type*)new TypeDefType($2, $4)));}
    | "namespace" IDENTIFIER docspec { ctx->current_namespace = ctx->current_namespace->get_or_add_namespace($2);} '{' slist '}' { ctx->current_namespace = ctx->current_namespace->get_parent();} ';'
    | IDENTIFIER ':' type ';' {ctx->current_namespace->add_export($1, $3);}
    ;

enum_body
    : enum_body ',' IDENTIFIER docspec { $1.push_back($3); $$ = $1; }
    | enum_body ',' { $$ = $1; }
    | IDENTIFIER docspec { $$ = std::vector<std::string>({$1}); }
    | %empty { $$ = std::vector<std::string>(); }
    ;

declaration_list
    : declaration_list ',' IDENTIFIER ':' type docspec { $1.push_back({$3, $5}); $$ = $1; }
    | declaration_list ',' { $$ = $1; }
    | IDENTIFIER ':' type docspec { $$ = std::vector<Declaration>({{$1, $3}}); }
    | %empty { $$ = std::vector<Declaration>(); }
    ;

docspec
    : '|' "depricated"
    | %empty
    ;

type
    : TYPE
    | type '?'
    { $$ = std::shared_ptr<Type>((Type*)new OptionalType($1)); }
    | type '[' ']'
    { $$ = std::shared_ptr<Type>((Type*)new ArrayType($1)); }
    | '(' declaration_list ')' '-' '>' '(' declaration_list ')'
    { $$ = std::shared_ptr<Type>((Type*)new FunctionType($2, $7)); }
    ;

%%


void pigeon::Parser::error(const location_type& l, const std::string& message)
{
    std::cerr << l.begin.filename->c_str() << ':' << l.begin.line << ':' << l.begin.column << '-' <<
                l.end.column << ": " << message << '\n';
}


pigeon::Parser::symbol_type pigeon::yylex(Context *ctx)
{
    const char *s, *e;
    /*!stags:re2c format = 'const char *@@;\n'; */
    for (;;)
    {
        %{
        // Configuration
        re2c:yyfill:enable  = 0;
        re2c:api:style = free-form;
        re2c:define:YYCTYPE = char;
        re2c:define:YYCURSOR = ctx->cur;
        re2c:define:YYMARKER = ctx->mar;
        re2c:define:YYLIMIT = ctx->lim;
        re2c:eof = 0;
        re2c:tags = 1;

        'enum' { TOKEN(ENUM); }
        'use' { TOKEN(USE); }
        'as' { TOKEN(AS); }
        'struct' { TOKEN(STRUCT); }
        'depricated' { TOKEN(DEPRICATED); }
        'namespace' { TOKEN(NAMESPACE); }

        @s [a-zA-Z_][a-zA-Z_0-9]* @e {
            std::string name(s, e - s);
            if (std::optional<std::shared_ptr<Type>> type = ctx->current_namespace->get_type(name))
            {
                TOKENV(TYPE, type.value());
            }
            TOKENV(IDENTIFIER, name);
        }

        // Whitespace
        $              { TOKEN(END_OF_FILE); }
        "\r\n"|[\r\n]  { ctx->loc.lines(); ctx->loc.step(); continue; }
        [ \t\v\b\f]    { ctx->loc.columns(); continue; }

        // Comment
        "//"[^\r\n]*   { continue; }

        // Default
        *              { return pigeon::Parser::symbol_type (ctx->cur[-1], ctx->loc); }
        %}
    }
}


bool pigeon::Context::interpret(char *input, unsigned int size, std::string input_filename, bool debug) {
        pigeon::location::filename_type filename(input_filename);

        this->loc = pigeon::location(&filename);

        this->cur = input;
        this->mar = input;
        this->lim = input + size;

        pigeon::Parser parser(this);
        if (debug) {
            parser.set_debug_level(1);
        }
        return !parser.parse();
}


bool pigeon::Context::interpret(std::istream& input_stream, std::string input_filename, bool debug) {
        input_stream.seekg(0, std::ios::end);
        std::streamsize size = input_stream.tellg();
        input_stream.seekg(0, std::ios::beg);

        std::vector<char> buffer((unsigned int)size);
        if (!input_stream.read(buffer.data(), size)) return false;
        buffer.push_back('\0');

        return interpret(buffer.data(), (unsigned int)size, input_filename, debug);
}


int main(int argc, char const *argv[])
{
    CLI::App app{"Pigeon"};
    std::string global_namespace_name;
    std::string output_filename;
    std::vector<std::string> input_filenames;
    bool debug = false;
    app.add_option("-g,--global", global_namespace_name, "Global namespace name")->required();
    app.add_option("-o,--output", output_filename, "Output file");
    app.add_option("input_files", input_filenames, "Input files");
    app.add_flag("--debug,!--no-debug", debug, "Debug");


    CLI11_PARSE(app, argc, argv);

    pigeon::Context context(global_namespace_name);

    for (auto input_filename : input_filenames)
    {
        std::ifstream file(input_filename, std::ios::binary);

        if (!context.interpret(file, input_filename, debug)) return 1;
    }

    context.dump_c(std::cout);

    return 0;
}
