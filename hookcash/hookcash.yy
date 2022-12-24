%no-lines
%require "3.4.1"
%language "c++"

%skeleton "lalr1.cc"

%define api.token.constructor
%define api.value.type variant
%define api.location.file none
%define parse.assert
%define parse.error verbose
%define parse.trace

%locations

%code requires
{
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "hookcash.hpp"
}//%code requires

%code
{
namespace yy { parser::symbol_type yylex(); }
static yy::location loc;

#define TOKEN(name) do { return yy::parser::make_##name(loc); } while(0)
#define TOKENV(name, ...) do { return yy::parser::make_##name(__VA_ARGS__, loc); } while(0)
#define HC_ASSERT(cond, loc, msg) do { if (!cond) { yy::parser::error(loc, msg); YYERROR; } } while(0)

static struct
{
    char *cur;
    char *mar;
    char *lim;
    Language mode = Language::HOOKCASH;
} in;

Context ctx;
}//%code

%token END_OF_FILE 0

%token IDENTIFIER LITERAL STRING REQUIRED_BOOL VARIABLE_TYPE
%token MODULE SET VARIANT SYMBOL ADDRESS
%token SEMICOLON

%type<std::string> IDENTIFIER STRING
%type<unsigned long long> LITERAL
%type<bool> REQUIRED_BOOL
%type<VariableType> VARIABLE_TYPE

%start slist

%%

slist
    : slist stmt
    | %empty
    ;

stmt
    : MODULE IDENTIFIER STRING SEMICOLON
    {
        HC_ASSERT(!ctx.modules.count($2), @1, "module \"" + $2 + "\" already exists.");
        ctx.modules[$2] = Module { $3 };
    }
    | SET IDENTIFIER VARIABLE_TYPE STRING SEMICOLON
    {
        HC_ASSERT(ctx.modules.count($2), @1, "module \"" + $2 + "\" does not exist.");
        ctx.modules[$2].set($3, $4);
    }
    | VARIANT IDENTIFIER IDENTIFIER STRING SEMICOLON
    {
        HC_ASSERT(ctx.modules.count($2), @1, "module \"" + $2 + "\" does not exist.");
        HC_ASSERT(!ctx.modules[$2].variants.count($3), @1, "module \"" + $2 + "\" already has variant \"" + $3 + "\".");
        ctx.modules[$2].variants[$3] = $4;
    }
    | SYMBOL IDENTIFIER IDENTIFIER REQUIRED_BOOL STRING SEMICOLON
    {
        HC_ASSERT(ctx.modules.count($2), @1, "module \"" + $2 + "\" does not exist.");
        HC_ASSERT(!ctx.modules[$2].symbols.count($3), @1, "module \"" + $2 + "\" already has symbol \"" + $3 + "\".");
        ctx.modules[$2].symbols[$3] = Symbol { $4, $5 };
    }
    | ADDRESS IDENTIFIER IDENTIFIER IDENTIFIER STRING SEMICOLON
    {
        HC_ASSERT(ctx.modules.count($2), @1, "module \"" + $2 + "\" does not exist.");
        HC_ASSERT(ctx.modules[$2].symbols.count($3), @1, "module \"" + $2 + "\" does not have symbol \"" + $3 + "\".");
        HC_ASSERT(ctx.modules[$2].variants.count($4), @1, "module \"" + $2 + "\" does not have variant \"" + $3 + "\".");
        HC_ASSERT(!ctx.modules[$2].symbols[$3].addresses.count($4), @1, "module \"" + $2 + "\" already has address for symbol \"" + $3 + "\" for variant \"" + $4 + "\".");
        ctx.modules[$2].symbols[$3].addresses[$4] = $5;
    }
    ;

%%

void yy::parser::error(const location_type& l, const std::string& message)
{
    std::cerr << l.begin.filename->c_str() << ':' << l.begin.line << ':' << l.begin.column << '-' << l.end.column << ": " << message << '\n';
}

yy::parser::symbol_type parse_literal(const std::string& str, int base)
{
    char *p;
    unsigned long long n = strtoull(str.c_str(), &p, base);
    if (*p != 0) TOKEN(YYerror);
    TOKENV(LITERAL, n);
}

static bool first_loop = true;
yy::parser::symbol_type yy::yylex()
{
    if (first_loop && in.mode == Language::SOURCE_CODE) { first_loop = false; goto source; }
    const char *s, *e;
    /*!stags:re2c format = 'const char *@@;\n'; */
    for (;;)
    {
        %{
        // Configuration
        re2c:yyfill:enable  = 0;
        re2c:api:style = free-form;
        re2c:define:YYCTYPE = char;
        re2c:define:YYCURSOR = in.cur;
        re2c:define:YYMARKER = in.mar;
        re2c:define:YYLIMIT = in.lim;
        re2c:eof = 0;
        re2c:tags = 1;
        %}
    hookcash:
        %{
        // Keywords
        'module'       { TOKEN(MODULE); }
        'set'          { TOKEN(SET); }
        'variant'      { TOKEN(VARIANT); }
        'symbol'       { TOKEN(SYMBOL); }
        'address'      { TOKEN(ADDRESS); }

        'required'     { TOKENV(REQUIRED_BOOL, true); }
        'optional'     { TOKENV(REQUIRED_BOOL, false); }

        'base_address' { TOKENV(VARIABLE_TYPE, VariableType::base_address); }

        ';'            { TOKEN(SEMICOLON); }

        // Identifier
        @s [a-zA-Z_][a-zA-Z_0-9]* @e { TOKENV(IDENTIFIER, std::string(s, e - s)); }

        // String
        // \x22 == '\"' I couldn't figure out how to use double quotes without someone complaining
        [\x22] @s [^\x22]* @e [\x22] { TOKENV(STRING, std::string(s, e - s)); }

        // Literal
        '0b' @s [0-1]+ @e       { return parse_literal(std::string(s, e - s).c_str(), 2); }
        '0x' @s [0-9a-fA-F]+ @e { return parse_literal(std::string(s, e - s).c_str(), 16); }
        @s [0-9]+ @e            { return parse_literal(std::string(s, e - s).c_str(), 10); }

        // Whitespace
        $              { TOKEN(END_OF_FILE); }
        "\r\n"|[\r\n]  { loc.lines(); loc.step(); if (in.mode == Language::SOURCE_CODE) goto source; continue; }
        [ \t\v\b\f]    { loc.columns(); continue; }

        // Comment
        "//"[^\r\n]*   { continue; }

        *              { TOKEN(YYerror); }
        %}
    source:
        %{
        $                                  { TOKEN(END_OF_FILE); }
        [ \t\v\b\f]* "//" [ \t\v\b\f]* "$" { goto hookcash; }
        *                                  { loc.columns(); goto source_internal; }
        %}
    source_internal:
        %{
        $             { TOKEN(END_OF_FILE); }
        "\r\n"|[\r\n] { loc.lines(); loc.step(); goto source; }
        [ \t\v\b\f]   { loc.columns(); goto source_internal; }
        *             { loc.columns(); goto source_internal; }
        %}
    }
}

std::unordered_map<std::string, Language> const EXTENSIONS = {
    { ".hc", Language::HOOKCASH },
    { ".c", Language::SOURCE_CODE },
    { ".cpp", Language::SOURCE_CODE },
    { ".h", Language::SOURCE_CODE },
    { ".hpp", Language::SOURCE_CODE },
    { ".hb", Language::BINARY },
};

int main(int argc, char const* argv[]) {
    if (argc < 2) return 1;

    for (int i = 1; i < argc; ++i) {
        std::ifstream file(argv[i], std::ios::binary | std::ios::ate);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if (!file.read(buffer.data(), size)) return 2;
        buffer.push_back('\0');

        yy::location::filename_type filename(argv[i]);

        loc = yy::location(&filename);

        in.cur = buffer.data();
        in.mar = buffer.data();
        in.lim = buffer.data() + size;

        std::filesystem::path file_path(argv[i]);
        std::string file_extension(file_path.extension().string());

        if (!EXTENSIONS.count(file_extension))
        {
            std::cerr << "Unable to determine file type of \"" << argv[i] << "\"\n";
            return 4;
        }
        in.mode = EXTENSIONS.at(file_extension);

        first_loop = true;
        yy::parser parser;
//    parser.set_debug_level(1);
        if (parser.parse()) return 3;
    }
    ctx.dump();
    return 0;
}