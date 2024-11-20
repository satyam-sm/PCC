%{
#include <stdio.h>
#include <stdlib.h>
%}

%token NUMBER
%token ADD SUB MUL DIV
%token LPAREN RPAREN

%left ADD SUB
%left MUL DIV
%right UMINUS

%%

input:
    | input line
;

line:
    '\n'
    | expression '\n'  { printf("Result: %d\n", $1); }
;

expression:
      NUMBER              { $$ = $1; }
    | expression ADD expression { $$ = $1 + $3; }
    | expression SUB expression { $$ = $1 - $3; }
    | expression MUL expression { $$ = $1 * $3; }
    | expression DIV expression { $$ = $1 / $3; }
    | SUB expression %prec UMINUS { $$ = -$2; }
    | LPAREN expression RPAREN   { $$ = $2; }
;

%%

int main() {
    printf("Enter expressions, or Ctrl+C to quit:\n");
    return yyparse();
}

int yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}
