%{
#include <stdio.h>
#include <stdlib.h>
%}

%option noyywrap

/* Token definitions */
%{
#define KEYWORD 1
#define IDENTIFIER 2
#define NUMBER 3
#define STRING_LITERAL 4
#define OPERATOR 5
#define PUNCTUATION 6
%}

/* Regular expressions for tokens */
%%
auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while   { printf("KEYWORD: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*  { printf("IDENTIFIER: %s\n", yytext); }
[0-9]+                   { printf("NUMBER: %s\n", yytext); }
\"([^\\\"]|\\.)*\"       { printf("STRING_LITERAL: %s\n", yytext); }
[+\-*/=<>!&|]+           { printf("OPERATOR: %s\n", yytext); }
[(){};,]                 { printf("PUNCTUATION: %s\n", yytext); }
[ \t\n]+                 ; /* Ignore whitespace */
.                        { printf("UNKNOWN: %s\n", yytext); }

%%

// Main function
int main(int argc, char **argv) {
    yylex();  // Call the lexer
    return 0;
}

int yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}