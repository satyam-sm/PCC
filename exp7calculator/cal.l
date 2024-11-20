%{
#include <stdio.h>
#include "cal.tab.h"
#include <stdlib.h> // For atoi()
%}

/* Rule Section */
%%
[0-9]+      {
                yylval = atoi(yytext);
                return NUMBER;
            }

[\t]        ;  // Ignore tabs
[\n]        return 0;  // End input on newline
.           return yytext[0];  // Return single character operators
%%

/* Driver Section */
int yywrap() {
    return 1;
}
