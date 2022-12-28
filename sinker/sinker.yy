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
#include <istream>

#include "sinker.hpp"

// Bison generates weird switch statements
#ifdef _MSC_VER
#pragma warning( disable : 4065 )
#endif

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

Context *ctx;

static bool in_pattern_match_expression = false;

}//%code

%token END_OF_FILE 0

%token IDENTIFIER INTEGER STRING BOOL PATTERN_BYTE
%token MODULE "module"
%token VARIANT "variant"
%token SYMBOL "symbol"
%token ADDRESS "address"
%token SET "set"
%token DOUBLE_COLON "::"

%type<std::string> IDENTIFIER STRING
%type<expression_value_t> INTEGER
%type<pattern_byte> PATTERN_BYTE
%type<Expression*> expression
%type<bool> BOOL
%type<attribute_value_t> attribute_value
%type<std::vector<pattern_byte>> pattern_match_expression

%left '+' '-'
%left '*'
%right INDIRECTION '@' '?' '!'
%left '[' '{'

%start slist

%%

slist
    : slist stmt
    | %empty
    ;

pattern_match_expression
    : pattern_match_expression PATTERN_BYTE
    {
        $1.push_back($2);
        $$ = $1;
    }
    | %empty { $$ = std::vector<pattern_byte>(); }
    ;

expression
    : INTEGER                          { $$ = (Expression*)new IntegerExpression($1);            }
    | '(' expression ')'               { $$ = (Expression*) new ParenthesesExpression($2);       }
    | expression '+' expression        { $$ = (Expression*)new AdditionExpression($1, $3);       }
    | expression '*' expression        { $$ = (Expression*)new MultiplicationExpression($1, $3); }
    | expression '-' expression        { $$ = (Expression*)new SubtractionExpression($1, $3);    }
    | '*' expression %prec INDIRECTION { $$ = (Expression*)new IndirectionExpression($2);        }
    | '@' expression                   { $$ = (Expression*)new RelocateExpression($2);           }
    | '?' expression                   { $$ = (Expression*)new NullCheckExpression($2);          }
    | expression '[' expression ']'    { $$ = (Expression*)new ArraySubscriptExpression($1, $3); }
    | '!' IDENTIFIER "::" IDENTIFIER
    {
        SINKER_ASSERT(ctx->get_module($2), @2, "Module does not exist");
        $$ = (Expression*)new GetProcAddressExpression(ctx->get_module($2), $4);
    }
    | IDENTIFIER
    {
        SINKER_ASSERT(ctx->get_module($1), @1, "Module does not exist");
        $$ = (Expression*)new ModuleExpression(ctx->get_module($1));
    }
    | IDENTIFIER "::" IDENTIFIER
    {
        SINKER_ASSERT(ctx->get_module($1), @1, "Module does not exist");
        SINKER_ASSERT(ctx->get_module($1)->get_symbol($3), @3, "Symbol does not exist");
        $$ = (Expression*)new SymbolExpression(ctx->get_module($1)->get_symbol($3));
    }
    | '{' {in_pattern_match_expression = true;} pattern_match_expression {in_pattern_match_expression = false;} '}'
    {
        $$ = (Expression*)new PatternMatchExpression($3);
    }
    ;

attribute_value
    : INTEGER { $$ = attribute_value_t {$1}; }
    | STRING  { $$ = attribute_value_t {$1}; }
    | BOOL    { $$ = attribute_value_t {$1}; }
    ;

stmt
    : "module" IDENTIFIER ',' STRING ',' INTEGER ';'
    {
        SINKER_ASSERT(!ctx->get_module($2), @2, "Module exists");
        ctx->emplace_module($2, $4, $6);
    }
    | "module" IDENTIFIER ',' INTEGER ';'
    {
        SINKER_ASSERT(!ctx->get_module($2), @2, "Module exists");
        ctx->emplace_module($2, {}, $4);
    }
    | "variant" IDENTIFIER ',' IDENTIFIER ',' STRING ';'
    {
        SINKER_ASSERT(ctx->get_module($2), @2, "Module does not exist");
        SINKER_ASSERT(!ctx->get_module($2)->has_variant($4), @4, "Variant exists");
        ctx->get_module($2)->add_variant($4, $6);
    }
    | "symbol" IDENTIFIER "::" IDENTIFIER ',' STRING ';'
    {
        SINKER_ASSERT(ctx->get_module($2), @2, "Module does not exist");
        SINKER_ASSERT(!ctx->get_module($2)->get_symbol($4), @4, "Symbol exists");
        ctx->get_module($2)->emplace_symbol($4, $6);
    }
    | "address" IDENTIFIER "::" IDENTIFIER ',' expression ';'
    {
        SINKER_ASSERT(ctx->get_module($2), @2, "Module does not exist");
        SINKER_ASSERT(ctx->get_module($2)->get_symbol($4), @4, "Symbol does not exist");
        ctx->get_module($2)->get_symbol($4)->add_address({}, $6);
    }
    | "address" IDENTIFIER "::" IDENTIFIER ',' IDENTIFIER ',' expression ';'
    {
        SINKER_ASSERT(ctx->get_module($2), @2, "Module does not exist");
        SINKER_ASSERT(ctx->get_module($2)->get_symbol($4), @4, "Symbol does not exist");
        ctx->get_module($2)->get_symbol($4)->add_address($6, $8);
    }
    | "set" IDENTIFIER ',' IDENTIFIER ',' attribute_value ';'
    {
        SINKER_ASSERT(ctx->get_module($2), @2, "Module does not exist");
        ctx->get_module($2)->set_attribute($4, $6);
    }
    | "set" IDENTIFIER "::" IDENTIFIER ',' IDENTIFIER ',' attribute_value ';'
    {
        SINKER_ASSERT(ctx->get_module($2), @2, "Module does not exist");
        SINKER_ASSERT(ctx->get_module($2)->get_symbol($4), @4, "Symbol does not exist");
        ctx->get_module($2)->get_symbol($4)->set_attribute($6, $8);
    }
    ;

%%

void yy::parser::error(const location_type& l, const std::string& message)
{
    std::cerr << l.begin.filename->c_str() << ':' << l.begin.line << ':' << l.begin.column << '-' <<
                l.end.column << ": " << message << '\n';
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
    sinker:
        if (in_pattern_match_expression) goto pattern_match;
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
        @s [;,()?@*-+{}!] | "[" | "]" { return yy::parser::symbol_type (*s, loc); }

        // Identifier
        @s [a-zA-Z_][a-zA-Z_0-9]* @e { TOKENV(IDENTIFIER, std::string(s, e - s)); }

        // String
        // \x22 == '\"' I couldn't figure out how to use double quotes without someone complaining
        [\x22] @s [^\x22]* @e [\x22] { TOKENV(STRING, std::string(s, e - s)); }

        // Literal
        '0b' @s [0-1]+ @e       { return parse_integer(std::string(s, e - s).c_str(), 2); }
        '0x' @s [0-9a-fA-F]+ @e { return parse_integer(std::string(s, e - s).c_str(), 16); }
        '0' @s [0-9a-fA-F]+ @e { return parse_integer(std::string(s, e - s).c_str(), 8); }
        @s [0-9]+ @e            { return parse_integer(std::string(s, e - s).c_str(), 10); }

        // Whitespace
        $              { TOKEN(END_OF_FILE); }
        "\r\n"|[\r\n]  { loc.lines(); loc.step(); if (in.mode == Language::SOURCE_CODE) goto source; continue; }
        [ \t\v\b\f]    { loc.columns(); continue; }

        // Comment
        "//"[^\r\n]*   { continue; }

        *              { std::cout << "bad character\n"; TOKEN(YYerror); }
        %}
    pattern_match:
        %{
        '??' { TOKENV(PATTERN_BYTE, { 0, 0x00 }); }
        @s [0-9a-fA-F][0-9a-fA-F] @e {
            char *p;
            unsigned long long n = strtoull(std::string(s, e - s).c_str(), &p, 16);
            if (*p != 0) TOKEN(YYerror);
            TOKENV(PATTERN_BYTE, { (std::uint8_t)n, 0xFF });
        }
        '}' { return yy::parser::symbol_type ('}', loc); }

        // Whitespace
        $              { TOKEN(END_OF_FILE); }
        "\r\n"|[\r\n]  { loc.lines(); loc.step(); if (in.mode == Language::SOURCE_CODE) goto source; continue; }
        [ \t\v\b\f]    { loc.columns(); continue; }

        // Comment
        "//"[^\r\n]*   { continue; }

        *              { std::cout << "bad character\n"; TOKEN(YYerror); }
        %}
    source:
        %{
        $                                  { TOKEN(END_OF_FILE); }
        [ \t\v\b\f]* "//" [ \t\v\b\f]* "$" { goto sinker; }
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

bool Context::interpret(std::istream& input_stream, Language language, std::string input_filename, bool debug) {
        ctx = this;
        input_stream.seekg(0, std::ios::end);
        std::streamsize size = input_stream.tellg();
        input_stream.seekg(0, std::ios::beg);

        std::vector<char> buffer((unsigned int)size);
        if (!input_stream.read(buffer.data(), size)) return false;
        buffer.push_back('\0');

        yy::location::filename_type filename(input_filename);

        loc = yy::location(&filename);

        in.cur = buffer.data();
        in.mar = buffer.data();
        in.lim = buffer.data() + size;

        in.mode = language;

        first_loop = true;
        in_pattern_match_expression = false;
        yy::parser parser;
        if (debug) {
            parser.set_debug_level(1);
        }
        return !parser.parse();
}

