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

%token IDENTIFIER
%token ENUM "enum"
%token STRUCT "struct"
%token DEPRICATED "depricated"

%type<std::string> IDENTIFIER

%code requires
{
#include <iostream>
#include <CLI/CLI.hpp>

// Bison generates weird switch statements
#ifdef _MSC_VER
#pragma warning( disable : 4065 )
#endif
}

%code top
{
    namespace pigeon { class Context; }
}

%code
{
#define TOKEN(name) do { return pigeon::Parser::make_##name(ctx->loc); } while(0)
#define TOKENV(name, ...) do { return pigeon::Parser::make_##name(__VA_ARGS__, ctx->loc); } while(0)
#define PIGEON_ASSERT(cond, loc, msg) do { if (!cond) { pigeon::Parser::error(loc, msg); YYERROR; } } while(0)


namespace pigeon
{
    Parser::symbol_type yylex(Context *context);

    class Context
    {
    public:
        bool interpret(char *input, unsigned int size, std::string input_filename, bool debug = false);
        bool interpret(std::istream& input_stream, std::string input_filename, bool debug);
        char *cur;
        char *mar;
        char *lim;
        location loc;
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
    : "enum" IDENTIFIER docspec '{' enum_body '}' ';'
    | "struct" IDENTIFIER docspec '{' struct_body '}' ';'
    ;

enum_body
    : enum_body ',' IDENTIFIER docspec
    | enum_body ','
    | IDENTIFIER docspec
    | %empty
    ;

struct_body
    : struct_body ',' IDENTIFIER ':' IDENTIFIER
    | struct_body ','
    | IDENTIFIER ':' IDENTIFIER
    | %empty
    ;

docspec
    : '|' "depricated"
    | %empty
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
        'struct' { TOKEN(STRUCT); }
        'depricated' { TOKEN(DEPRICATED); }

        @s [a-zA-Z_][a-zA-Z_0-9]* @e { TOKENV(IDENTIFIER, std::string(s, e - s)); }

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

    pigeon::Context context;    

    for (auto input_filename : input_filenames)
    {
        std::ifstream file(input_filename, std::ios::binary);

        if (!context.interpret(file, input_filename, debug)) return 1;
    }

    std::cout << "Hello, World!\n";

    return 0;
}
