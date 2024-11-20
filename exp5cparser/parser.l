%{
#include <stdio.h>
#include "parser.tab.h"
%}

%%
"int"|"float"|"char"|"double" { return TYPE; }
"if"                         { return IF; }
"else"                       { return ELSE; }
"while"                      { return WHILE; }
"return"                     { return RETURN; }
[0-9]+                       { yylval = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*        { yylval = strdup(yytext); return IDENTIFIER; }
"="                          { return ASSIGN; }
"=="|"!="|"<"|">"            { return RELOP; }
"("                          { return LPAREN; }
")"                          { return RPAREN; }
"{"                          { return LBRACE; }
"}"                          { return RBRACE; }
";"                          { return SEMICOLON; }
"\\+"|"-"|"\\*"|"/"          { return OPERATOR; }
[ \t\n]+                     ; // Ignore whitespace
.                            { return yytext[0]; }
%%
