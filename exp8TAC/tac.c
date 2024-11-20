#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char expr[100];
    char temp[10];
    int tCount = 1, i = 0, len;
    char id1[10], id2[10], op;

    printf("Enter the Expression: ");
    scanf("%s", expr);

    len = strlen(expr);

    printf("Three Address Code:\n");

    while (i < len) {
        // Check for higher precedence operators first (* or /)
        if (expr[i] == '*' || expr[i] == '/') {
            op = expr[i];
            id1[0] = expr[i - 1];
            id1[1] = '\0';
            id2[0] = expr[i + 1];
            id2[1] = '\0';

            printf("T%d = %s %c %s\n", tCount, id1, op, id2);

            // Replace the result in the expression
            sprintf(temp, "T%d", tCount++);
            expr[i - 1] = temp[0];
            for (int j = i; j < len - 2; j++) {
                expr[j] = expr[j + 2];
            }
            len -= 2;
            expr[len] = '\0';
            i = 0; // Reset index to handle new precedence
        } else {
            i++;
        }
    }

    // Handle lower precedence operators (+ or -)
    i = 0;
    while (i < len) {
        if (expr[i] == '+' || expr[i] == '-') {
            op = expr[i];
            id1[0] = expr[i - 1];
            id1[1] = '\0';
            id2[0] = expr[i + 1];
            id2[1] = '\0';

            printf("T%d = %s %c %s\n", tCount, id1, op, id2);

            // Replace the result in the expression
            sprintf(temp, "T%d", tCount++);
            expr[i - 1] = temp[0];
            for (int j = i; j < len - 2; j++) {
                expr[j] = expr[j + 2];
            }
            len -= 2;
            expr[len] = '\0';
            i = 0; // Reset index to handle new precedence
        } else {
            i++;
        }
    }

    return 0;
}
