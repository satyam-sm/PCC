%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char *s);
extern int yylex();
%}

%token TYPE IDENTIFIER NUMBER IF ELSE WHILE RETURN ASSIGN RELOP OPERATOR LPAREN RPAREN LBRACE RBRACE SEMICOLON

%%
program: 
    declarations statements
;

declarations: 
    declarations declaration
    | /* empty */
;

declaration: 
    TYPE IDENTIFIER SEMICOLON
    | TYPE IDENTIFIER ASSIGN expression SEMICOLON // Variable initialization
;

statements: 
    statements statement
    | /* empty */
;

statement:
    assignment
    | conditional
    | loop
    | block
    | RETURN expression SEMICOLON
;

assignment: 
    IDENTIFIER ASSIGN expression SEMICOLON
;

conditional: 
    IF LPAREN expression RPAREN statement
    | IF LPAREN expression RPAREN statement ELSE statement
;

loop: 
    WHILE LPAREN expression RPAREN statement
;

block: 
    LBRACE statements RBRACE
;

expression: 
    expression OPERATOR expression
    | IDENTIFIER
    | NUMBER
    | LPAREN expression RPAREN
;
%%
int main() {
    printf("Enter C-like code:\n");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
