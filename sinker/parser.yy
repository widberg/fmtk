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

%define api.namespace {sinker}
%define api.parser.class {Parser}

%parse-param {Context *ctx}
%param {LexerState *lexer_state}

%locations

%code requires
{
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <istream>

#include "sinker.hpp"
using namespace sinker;

// Bison generates weird switch statements
#ifdef _MSC_VER
#pragma warning( disable : 4065 )
#endif

}//%code requires


%code top
{
    
struct LexerState
{
    bool first_loop = true;
    bool in_pattern_match_expression = false;
};
}

%code
{
namespace sinker { Parser::symbol_type yylex(LexerState *lexer_state); }
static sinker::location loc;

#define TOKEN(name) do { return sinker::Parser::make_##name(loc); } while(0)
#define TOKENV(name, ...) do { return sinker::Parser::make_##name(__VA_ARGS__, loc); } while(0)
#define SINKER_ASSERT(cond, loc, msg) do { if (!cond) { sinker::Parser::error(loc, msg); YYERROR; } } while(0)

static struct
{
    char *cur;
    char *mar;
    char *lim;
    Language mode = Language::SINKER;
} in;

}//%code

%initial-action
{
lexer_state->first_loop = true;
lexer_state->in_pattern_match_expression = false;
}//%initial-action

%token END_OF_FILE 0

%token IDENTIFIER INTEGER STRING BOOL PATTERN_BYTE
%token MODULE "module"
%token VARIANT "variant"
%token SYMBOL "symbol"
%token ADDRESS "address"
%token SET "set"

%type<std::string> IDENTIFIER STRING
%type<expression_value_t> INTEGER
%type<pattern_byte> PATTERN_BYTE
%type<std::shared_ptr<Expression>> expression
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
    : INTEGER                          { $$ = std::shared_ptr<Expression>((Expression*)new IntegerExpression($1));            }
    | '(' expression ')'               { $$ = std::shared_ptr<Expression>((Expression*)new ParenthesesExpression($2));        }
    | expression '+' expression        { $$ = std::shared_ptr<Expression>((Expression*)new AdditionExpression($1, $3));       }
    | expression '*' expression        { $$ = std::shared_ptr<Expression>((Expression*)new MultiplicationExpression($1, $3)); }
    | expression '-' expression        { $$ = std::shared_ptr<Expression>((Expression*)new SubtractionExpression($1, $3));    }
    | '*' expression %prec INDIRECTION { $$ = std::shared_ptr<Expression>((Expression*)new IndirectionExpression($2));        }
    | '@' expression                   { $$ = std::shared_ptr<Expression>((Expression*)new RelocateExpression($2));           }
    | '?' expression                   { $$ = std::shared_ptr<Expression>((Expression*)new NullCheckExpression($2));          }
    | expression '[' expression ']'    { $$ = std::shared_ptr<Expression>((Expression*)new ArraySubscriptExpression($1, $3)); }
    | '!' IDENTIFIER ':' ':' IDENTIFIER
    {
        SINKER_ASSERT(ctx->get_module($2), @2, "Module does not exist");
        $$ = std::shared_ptr<Expression>((Expression*)new GetProcAddressExpression(ctx->get_module($2), $5));
    }
    | IDENTIFIER
    {
        SINKER_ASSERT(ctx->get_module($1), @1, "Module does not exist");
        $$ = std::shared_ptr<Expression>((Expression*)new ModuleExpression(ctx->get_module($1)));
    }
    | IDENTIFIER ':' ':' IDENTIFIER
    {
        SINKER_ASSERT(ctx->get_module($1), @1, "Module does not exist");
        SINKER_ASSERT(ctx->get_module($1)->get_symbol($4), @4, "Symbol does not exist");
        $$ = std::shared_ptr<Expression>((Expression*)new SymbolExpression(ctx->get_module($1)->get_symbol($4)));
    }
    | '{' {lexer_state->in_pattern_match_expression = true;} pattern_match_expression {lexer_state->in_pattern_match_expression = false;} '}'
    {
        $$ = std::shared_ptr<Expression>((Expression*)new PatternMatchExpression($3));
    }
    ;

attribute_value
    : INTEGER { $$ = attribute_value_t {$1}; }
    | STRING  { $$ = attribute_value_t {$1}; }
    | BOOL    { $$ = attribute_value_t {$1}; }
    ;

stmt
    : "module" IDENTIFIER ',' STRING ';'
    {
        SINKER_ASSERT(!ctx->get_module($2), @2, "Module exists");
        ctx->emplace_module($2, $4);
    }
    | "module" IDENTIFIER ';'
    {
        SINKER_ASSERT(!ctx->get_module($2), @2, "Module exists");
        ctx->emplace_module($2, {});
    }
    | "variant" IDENTIFIER ',' IDENTIFIER ',' STRING ';'
    {
        SINKER_ASSERT(ctx->get_module($2), @2, "Module does not exist");
        SINKER_ASSERT(!ctx->get_module($2)->has_variant($4), @4, "Variant exists");
        ctx->get_module($2)->add_variant($4, $6);
    }
    | "symbol" IDENTIFIER ':' ':' IDENTIFIER ',' STRING ';'
    {
        SINKER_ASSERT(ctx->get_module($2), @2, "Module does not exist");
        SINKER_ASSERT(!ctx->get_module($2)->get_symbol($5), @5, "Symbol exists");
        ctx->get_module($2)->emplace_symbol($5, $7);
    }
    | "address" IDENTIFIER ':' ':' IDENTIFIER ',' expression ';'
    {
        SINKER_ASSERT(ctx->get_module($2), @2, "Module does not exist");
        SINKER_ASSERT(ctx->get_module($2)->get_symbol($5), @5, "Symbol does not exist");
        ctx->get_module($2)->get_symbol($5)->add_address({}, $7);
    }
    | "address" IDENTIFIER ':' ':' IDENTIFIER ',' IDENTIFIER ',' expression ';'
    {
        SINKER_ASSERT(ctx->get_module($2), @2, "Module does not exist");
        SINKER_ASSERT(ctx->get_module($2)->get_symbol($5), @5, "Symbol does not exist");
        ctx->get_module($2)->get_symbol($5)->add_address($7, $9);
    }
    | "set" IDENTIFIER ',' IDENTIFIER ',' attribute_value ';'
    {
        SINKER_ASSERT(ctx->get_module($2), @2, "Module does not exist");
        ctx->get_module($2)->set_attribute($4, $6);
    }
    | "set" IDENTIFIER ':' ':' IDENTIFIER ',' IDENTIFIER ',' attribute_value ';'
    {
        SINKER_ASSERT(ctx->get_module($2), @2, "Module does not exist");
        SINKER_ASSERT(ctx->get_module($2)->get_symbol($5), @4, "Symbol does not exist");
        ctx->get_module($2)->get_symbol($5)->set_attribute($7, $9);
    }
    ;

%%

void sinker::Parser::error(const location_type& l, const std::string& message)
{
    std::cerr << l.begin.filename->c_str() << ':' << l.begin.line << ':' << l.begin.column << '-' <<
                l.end.column << ": " << message << '\n';
}

sinker::Parser::symbol_type parse_integer(const std::string& str, int base)
{
    char *p;
    unsigned long long n = strtoull(str.c_str(), &p, base);
    if (*p != 0) TOKEN(YYerror);
    TOKENV(INTEGER, n);
}

sinker::Parser::symbol_type sinker::yylex(LexerState *lexer_state)
{
    if (lexer_state->first_loop && in.mode == Language::SOURCE_CODE) { lexer_state->first_loop = false; goto source; }
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
        if (lexer_state->in_pattern_match_expression) goto pattern_match;
        %{
        // Keywords
        'module'       { TOKEN(MODULE); }
        'set'          { TOKEN(SET); }
        'variant'      { TOKEN(VARIANT); }
        'symbol'       { TOKEN(SYMBOL); }
        'address'      { TOKEN(ADDRESS); }

        'true'         { TOKENV(BOOL, true); }
        'false'        { TOKENV(BOOL, false); }

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

        *              { return sinker::Parser::symbol_type (in.cur[-1], loc); }
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
        '}' { return sinker::Parser::symbol_type ('}', loc); }

        // Whitespace
        $              { TOKEN(END_OF_FILE); }
        "\r\n"|[\r\n]  { loc.lines(); loc.step(); if (in.mode == Language::SOURCE_CODE) goto source; continue; }
        [ \t\v\b\f]    { loc.columns(); continue; }

        // Comment
        "//"[^\r\n]*   { continue; }

        *              { return sinker::Parser::symbol_type (in.cur[-1], loc); }
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

bool Context::interpret(char *input, unsigned int size, Language language, std::string input_filename, bool debug) {
        sinker::location::filename_type filename(input_filename);

        // TODO: add this stuff to the lexer context
        loc = sinker::location(&filename);

        in.cur = input;
        in.mar = input;
        in.lim = input + size;

        in.mode = language;
        LexerState lexer_state;
        sinker::Parser parser(this, &lexer_state);
        if (debug) {
            parser.set_debug_level(1);
        }
        return !parser.parse();
}

bool Context::interpret(std::istream& input_stream, Language language, std::string input_filename, bool debug) {
        input_stream.seekg(0, std::ios::end);
        std::streamsize size = input_stream.tellg();
        input_stream.seekg(0, std::ios::beg);

        std::vector<char> buffer((unsigned int)size);
        if (!input_stream.read(buffer.data(), size)) return false;
        buffer.push_back('\0');

        return interpret(buffer.data(), (unsigned int)size, language, input_filename, debug);
}
