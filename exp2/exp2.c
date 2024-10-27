#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct opTable {
    char code[10], objcode[10];
} myOpT[3] = {
    "LDA", "00",
    "STA", "0C",
    "LDCH", "50"
};

struct symbolTable {
    char symbol[10];
    int addr;
} mySymTab[10];

int startAddress, locCounter, symCount = 0, length;
char line[20], label[8], opcode[8], operand[8], programName[10];

void checkLabel() {
    int k, dupSymbol = 0;
    for (k = 0; k < symCount; k++)
        if (!strcmp(label, mySymTab[k].symbol)) {
            mySymTab[k].addr = -1;
            dupSymbol = 1;
            break;
        }
    if (!dupSymbol) {
        strcpy(mySymTab[symCount].symbol, label);
        mySymTab[symCount++].addr = locCounter;
    }
}

void checkOpCode() {
    int k = 0, found = 0;
    for (k = 0; k < 3; k++)
        if (!strcmp(opcode, myOpT[k].code)) {
            locCounter += 3;
            found = 1;
            break;
        }
    if (!found) {
        if (!strcmp(opcode, "WORD")) locCounter += 3;
        else if (!strcmp(opcode, "RESW")) locCounter += (3 * atoi(operand));
        else if (!strcmp(opcode, "RESB")) locCounter += atoi(operand);
    }
}

void readLine() {
    char buff[8], word1[8], word2[8], word3[8];
    int i, j = 0, count = 0;

    label[0] = opcode[0] = operand[0] = word1[0] = word2[0] = word3[0] = '\0';
    for (i = 0; line[i] != '\0'; i++) {
        if (line[i] != ' ') buff[j++] = line[i];
        else {
            buff[j] = '\0';
            strcpy(word3, word2);
            strcpy(word2, word1);
            strcpy(word1, buff);
            j = 0;
            count++;
        }
    }
    buff[j - 1] = '\0';
    strcpy(word3, word2);
    strcpy(word2, word1);
    strcpy(word1, buff);

    switch (count) {
        case 0:
            strcpy(opcode, word1);
            break;
        case 1: {
            strcpy(opcode, word2);
            strcpy(operand, word1);
        }
        break;
        case 2: {
            strcpy(label, word3);
            strcpy(opcode, word2);
            strcpy(operand, word1);
        }
        break;
    }
}

void PASS1() {
    FILE *input, *inter;
    input = fopen("assemblycode.txt", "r");
    inter = fopen("intermediate.txt", "w");

    printf("LOCATION LABEL\tOPERAND\tOPCODE\n");
    printf("_____________________________________");

    fgets(line, 20, input);
    readLine();

    if (!strcmp(opcode, "START")) {
        startAddress = atoi(operand);
        locCounter = startAddress;
        strcpy(programName, label);
        fprintf(inter, "%s", line);
        fgets(line, 20, input);
    } else {
        programName[0] = '\0';
        startAddress = 0;
        locCounter = 0;
    }

    printf("\n %d\t %s\t%s\t %s", locCounter, label, opcode, operand);
    while (strcmp(line, "END") != 0) {
        readLine();
        printf("\n %d\t %s \t%s\t %s", locCounter, label, opcode, operand);
        if (label[0] != '\0') checkLabel();
        checkOpCode();
        fprintf(inter, "%s %s %s\n", label, opcode, operand);
        fgets(line, 20, input);
    }

    printf("\n %d\t\t%s", locCounter, line);
    fprintf(inter, "%s", line);

    fclose(inter);
    fclose(input);
}

void PASS2() {
    FILE *inter, *output;
    char record[30], part[6], value[5];
    int currtxtlen = 0, foundopcode, foundoperand, chk, operandaddr, recaddr = 0;

    inter = fopen("intermediate.txt", "r");
    output = fopen("output.txt", "w");

    fgets(line, 20, inter);
    readLine();

    if (!strcmp(opcode, "START")) fgets(line, 20, inter);

    printf("\n\nObject Code\n");
    printf("\nH^ %s ^ %d ^ %d ", programName, startAddress, length);
    fprintf(output, "\nH^ %s ^ %d ^ %d ", programName, startAddress, length);

    recaddr = startAddress;
    record[0] = '\0';

    while (strcmp(line, "END") != 0) {
        operandaddr = foundoperand = foundopcode = 0;
        value[0] = part[0] = '\0';
        readLine();

        for (chk = 0; chk < 3; chk++) {
            if (!strcmp(opcode, myOpT[chk].code)) {
                foundopcode = 1;
                strcpy(part, myOpT[chk].objcode);
                if (operand[0] != '\0') {
                    for (chk = 0; chk < symCount; chk++)
                        if (!strcmp(mySymTab[chk].symbol, operand)) {
                            sprintf(value, "%d", mySymTab[chk].addr);
                            strcat(part, value);
                            foundoperand = 1;
                        }
                    if (!foundoperand) strcat(part, "err");
                }
            }
        }

        if (!foundopcode) {
            if (strcmp(opcode, "BYTE") == 0 || strcmp(opcode, "WORD") || strcmp(opcode, "RESB")) {
                strcat(part, operand);
            }
        }

        if ((currtxtlen + strlen(part)) <= 8) {
            strcat(record, "^");
            strcat(record, part);
            currtxtlen += strlen(part);
        } else {
            printf("\nT^ %d ^%d %s", recaddr, currtxtlen, record);
            fprintf(output, "\nT^ %d ^%d %s", recaddr, currtxtlen, record);
            recaddr += currtxtlen;
            currtxtlen = strlen(part);
            strcpy(record, part);
        }
        fgets(line, 20, inter);
    }

    printf("\nT^ %d ^%d %s", recaddr, currtxtlen, record);
    fprintf(output, "\nT^ %d ^%d %s", recaddr, currtxtlen, record);
    printf("\nE^ %d\n", startAddress);
    fprintf(output, "\nE^ %d\n", startAddress);

    fclose(inter);
    fclose(output);
}

int main() {
    PASS1();
    length = locCounter - startAddress;
    PASS2();
    // Removed getch() as it's not portable
    getchar(); // Waits for a keypress before exit
}
