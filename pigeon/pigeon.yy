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
%type<std::shared_ptr<pigeon::Type>> TYPE type
%type<std::vector<std::string>> enum_body

%code requires
{
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <optional>
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

    class Namespace
    {
    public:
        Namespace(std::string const& name, std::shared_ptr<Namespace> parent)
            : name(name), parent(parent) {}
        Namespace(std::string const& name)
            : name(name), parent(nullptr) {}
        
        std::optional<std::shared_ptr<Type>> get_type(std::string_view name) const {
            for (auto it : types) {
                if (it.first == name)
                {
                    return it.second;
                }
            }

            for (auto child : children)
            {
                if (auto type = child->get_type(name))
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

        
        std::shared_ptr<Namespace> emplace_namespace(std::string const& name)
        {
            children.emplace_back(std::make_shared<Namespace>(name), this);
            return children.back();
        }

        std::shared_ptr<Namespace> get_parent()
        {
            return parent;
        }

    private:
        std::string name;
        std::vector<std::shared_ptr<Namespace>> children;
        std::shared_ptr<Namespace> parent;
        std::vector<std::pair<std::string, std::shared_ptr<Type>>> types;
    };

    class Type
    {
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
        };

        PrimitiveType(Kind kind)
            : kind(kind) {}
    private:
        Kind kind;
    };

    class OptionalType : public Type
    {
    public:
        OptionalType(std::shared_ptr<Type> type)
            : type(type) {}
    private:
        std::shared_ptr<Type> type;
    };

    class ArrayType : public Type
    {
    public:
        ArrayType(std::shared_ptr<Type> type)
            : type(type) {}
    private:
        std::shared_ptr<Type> type;
    };
    
    class EnumType : public Type
    {
    public:
        EnumType(std::vector<std::string> const& values)
            : values(values) {}
    private:
        std::vector<std::string> values;
    };

    class Context
    {
    public:
        Context(std::string const& name)
            : global(std::make_shared<Namespace>(name)), current_namespace(global)
        {
            global->add_type("u8", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::U8)));
            global->add_type("u16", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::U16)));
            global->add_type("u32", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::U32)));
            global->add_type("u64", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::U64)));
            global->add_type("i8", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::I8)));
            global->add_type("i16", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::I16)));
            global->add_type("i32", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::I32)));
            global->add_type("i64", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::I64)));
            global->add_type("f32", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::F32)));
            global->add_type("f64", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::F64)));
            global->add_type("bool", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::BOOL)));
            global->add_type("handle", std::shared_ptr<Type>((Type*)new PrimitiveType(PrimitiveType::Kind::HANDLE)));
        }
        bool interpret(char *input, unsigned int size, std::string input_filename, bool debug = false);
        bool interpret(std::istream& input_stream, std::string input_filename, bool debug);
        char *cur;
        char *mar;
        char *lim;
        location loc;
        std::shared_ptr<Namespace> global;
        std::shared_ptr<Namespace> current_namespace;
    };

}


}

%right '['
%right '?'

%start slist

%%

slist
    : slist stmt
    | %empty
    ;

stmt
    : "enum" IDENTIFIER docspec '{' enum_body '}' { ctx->current_namespace->add_type($2, std::shared_ptr<Type>((Type*)new EnumType($5)));} ';'
    | "struct" IDENTIFIER docspec '{' struct_body '}' ';'
    | "use" type "as" IDENTIFIER docspec ';'
    | "namespace" IDENTIFIER docspec { ctx->current_namespace = ctx->current_namespace->emplace_namespace($2); } '{' slist '}' { ctx->current_namespace = ctx->current_namespace->get_parent(); } ';'
    ;

enum_body
    : enum_body ',' IDENTIFIER docspec{ $1.push_back($3); $$ = $1; }
    | enum_body ',' { $$ = $1; }
    | IDENTIFIER docspec { $$ = std::vector<std::string>({$1}); }
    | %empty { $$ = std::vector<std::string>(); }
    ;

struct_body
    : struct_body ',' IDENTIFIER ':' type
    | struct_body ','
    | IDENTIFIER ':' type
    | %empty
    ;

docspec
    : '|' "depricated"
    | %empty
    ;

type
    : TYPE
    | '?' type
    { $$ = std::shared_ptr<Type>((Type*)new pigeon::OptionalType($2)); }
    | type '[' ']'
    { $$ = std::shared_ptr<Type>((Type*)new pigeon::ArrayType($1)); }
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
    std::string output_filename;
    std::vector<std::string> input_filenames;
    bool debug = false;
    app.add_option("-o,--output", output_filename, "Output file");
    app.add_option("input_files", input_filenames, "Input files");
    app.add_flag("--debug,!--no-debug", debug, "Input files");


    CLI11_PARSE(app, argc, argv);

    pigeon::Context context("test");

    for (auto input_filename : input_filenames)
    {
        std::ifstream file(input_filename, std::ios::binary);

        if (!context.interpret(file, input_filename, debug)) return 1;
    }

    std::cout << "Hello, World!\n";

    return 0;
}
