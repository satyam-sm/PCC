%{
#include <stdio.h>
#include <stdlib.h> // For exit()
void yyerror(char *s);
%}

%token NUMBER
%left '+' '-'
%left '*' '/' '%'
%left '(' ')'

%%
ArithmeticExpression:
    E {
        printf("Answer: %d\n", $1);
        return 0;
    }
;

E:
    E '+' E { $$ = $1 + $3; }
    | E '-' E { $$ = $1 - $3; }
    | E '*' E { $$ = $1 * $3; }
    | E '/' E {
        if ($3 == 0) {
            yyerror("Division by zero!");
            exit(1);
        }
        $$ = $1 / $3;
    }
    | E '%' E {
        if ($3 == 0) {
            yyerror("Modulo by zero!");
            exit(1);
        }
        $$ = $1 % $3;
    }
    | '(' E ')' { $$ = $2; }
    | NUMBER { $$ = $1; }
;

%%
int main() {
    printf("Enter an expression: ");
    yyparse();
    return 0;
}

void yyerror(char *s) {
    printf("Error: %s\n", s);
}
