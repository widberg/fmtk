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
#include <format>

#include "sinker.hpp"
}//%code requires

%code
{
namespace yy { parser::symbol_type yylex(); }
static yy::location loc;

#define TOKEN(name) do { return yy::parser::make_##name(loc); } while(0)
#define TOKENV(name, ...) do { return yy::parser::make_##name(__VA_ARGS__, loc); } while(0)
#define SINKER_ASSERT(cond, loc, msg) do { if (!cond) { yy::parser::error(loc, msg); YYERROR; } } while(0)

static struct
{
    char *cur;
    char *mar;
    char *lim;
    Language mode = Language::SINKER;
} in;

}//%code

%token END_OF_FILE 0

%token IDENTIFIER INTEGER STRING BOOL
%token MODULE SET VARIANT SYMBOL ADDRESS
%token ';' ',' '(' ')'
%token DOUBLE_COLON "::"

%type<std::string> IDENTIFIER STRING
%type<unsigned long long> INTEGER expression
%type<bool> BOOL
%type<attribute_value_t> attribute_value

%start slist

%%

slist
    : slist stmt
    | %empty
    ;

expression
    : INTEGER

    ;

attribute_value
    : INTEGER
    {
        $$ = attribute_value_t {$1};
    }
    | STRING
    {
        $$ = attribute_value_t {$1};
    }
    | BOOL
    {
        $$ = attribute_value_t {$1};
    }
    ;

stmt
    : MODULE IDENTIFIER ',' STRING ',' INTEGER ';'
    {
        std::cout << std::format("module {}, {}, {}\n", $2, $4, $6);
    }
    | MODULE IDENTIFIER ',' INTEGER ';'
    {
        std::cout << std::format("module {}, {}\n", $2, $4);
    }
    | VARIANT IDENTIFIER ',' IDENTIFIER ',' STRING ';'
    {
        std::cout << std::format("variant {}, {}, {}\n", $2, $4, $6);
    }
    | SYMBOL IDENTIFIER "::" IDENTIFIER ',' STRING ';'
    {
        std::cout << std::format("symbol {}::{}, {}\n", $2, $4, $6);
    }
    | ADDRESS IDENTIFIER "::" IDENTIFIER ',' expression ';'
    {
        std::cout << std::format("address {}::{}, {}\n", $2, $4, $6);
    }
    | ADDRESS IDENTIFIER "::" IDENTIFIER ',' IDENTIFIER ',' expression ';'
    {
        std::cout << std::format("address {}::{}, {}, {}\n", $2, $4, $6, 8);
    }
    | SET IDENTIFIER ',' IDENTIFIER ',' attribute_value ';'
    {
        std::cout << std::format("set {}, {}, {}\n", $2, $4, $6);
    }
    | SET IDENTIFIER "::" IDENTIFIER ',' IDENTIFIER ',' attribute_value ';'
    {
        std::cout << std::format("set {}::{}, {}, {}\n", $2, $4, $6, $8);
    }
    ;

%%

void yy::parser::error(const location_type& l, const std::string& message)
{
    std::cerr << l.begin.filename->c_str() << ':' << l.begin.line << ':' << l.begin.column << '-' << l.end.column << ": " << message << '\n';
}

yy::parser::symbol_type parse_integer(const std::string& str, int base)
{
    char *p;
    unsigned long long n = strtoull(str.c_str(), &p, base);
    if (*p != 0) TOKEN(YYerror);
    TOKENV(INTEGER, n);
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

        'true'         { TOKENV(BOOL, true); }
        'false'        { TOKENV(BOOL, false); }

        '::'           { TOKEN(DOUBLE_COLON); }
        @s [;,()]            { return yy::parser::symbol_type (*s, loc); }

        // Identifier
        @s [a-zA-Z_][a-zA-Z_0-9]* @e { TOKENV(IDENTIFIER, std::string(s, e - s)); }

        // String
        // \x22 == '\"' I couldn't figure out how to use double quotes without someone complaining
        [\x22] @s [^\x22]* @e [\x22] { TOKENV(STRING, std::string(s, e - s)); }

        // Literal
        '0b' @s [0-1]+ @e       { return parse_integer(std::string(s, e - s).c_str(), 2); }
        '0x' @s [0-9a-fA-F]+ @e { return parse_integer(std::string(s, e - s).c_str(), 16); }
        @s [0-9]+ @e            { return parse_integer(std::string(s, e - s).c_str(), 10); }

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

        in.mode = file_extension == ".skr" ? Language::SINKER : Language::SOURCE_CODE;

        first_loop = true;
        yy::parser parser;
//    parser.set_debug_level(1);
        if (parser.parse()) return 3;
    }
    return 0;
}
