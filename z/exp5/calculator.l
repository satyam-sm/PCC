%{
#include "y.tab.h"
%}

%%

[0-9]+          { yylval = atoi(yytext); return NUMBER; }
[\t\n ]+        { /* Ignore whitespace */ }
"+"             { return ADD; }
"-"             { return SUB; }
"*"             { return MUL; }
"/"             { return DIV; }
"("             { return LPAREN; }
")"             { return RPAREN; }

.               { /* Catch-all for unknown characters */ }

%%

int yywrap() {
    return 1;
}
