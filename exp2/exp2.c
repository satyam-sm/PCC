#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct opTable {
    char code[10], objcode[10];
} myOpT[3] = {
    {"LDA", "00"},
    {"STA", "0C"},
    {"JMP", "50"}
};

struct symbolTable {
    char symbol[10];
    int addr;
} mySymTab[10];

int startAddress, locCounter, symCount = 0, length;
char line[20], label[20], opcode[20], operand[20], programName[20];

const char *assemblyCode[] = {
    "MYCODE START 1000",
    "STA",
    "LOOP1 JMP LOOP2",
    "LDA",
    "LOOP2 JMP LOOP1",
    "RESB 09",
    "LDA",
    "STA",
    "JMP LOOP1",
    "END"
};

int assemblyCodeSize = sizeof(assemblyCode) / sizeof(assemblyCode[0]);

void checkLabel() {
    for (int k = 0; k < symCount; k++) {
        if (!strcmp(label, mySymTab[k].symbol)) {
            mySymTab[k].addr = -1; // Duplicate symbol
            printf("Warning: Duplicate symbol '%s' detected.\n", label);
            return;
        }
    }
    if (symCount < 10) {
        strcpy(mySymTab[symCount].symbol, label);
        mySymTab[symCount++].addr = locCounter;
    } else {
        printf("Error: Symbol table overflow.\n");
        exit(1);
    }
}

void checkOpCode() {
    int found = 0;
    for (int k = 0; k < 3; k++) {
        if (!strcmp(opcode, myOpT[k].code)) {
            locCounter += 3;
            found = 1;
            break;
        }
    }
    if (!found) {
        if (!strcmp(opcode, "WORD"))
            locCounter += 3;
        else if (!strcmp(opcode, "RESW"))
            locCounter += (3 * atoi(operand));
        else if (!strcmp(opcode, "RESB"))
            locCounter += atoi(operand);
        else if (!strcmp(opcode, "BYTE"))
            locCounter += strlen(operand) - 3; // Length of the string literal
    }
}

void readLine(const char *line) {
    char buff[20] = "", word1[20] = "", word2[20] = "", word3[20] = "";
    int i, j = 0, count = 0;

    label[0] = opcode[0] = operand[0] = '\0';
    for (i = 0; line[i] != '\0'; i++) {
        if (line[i] != ' ') {
            buff[j++] = line[i];
        } else if (j > 0) {
            buff[j] = '\0';
            j = 0;
            count++;
            if (count == 1) strcpy(word1, buff);
            else if (count == 2) strcpy(word2, buff);
            else if (count == 3) strcpy(word3, buff);
        }
    }
    if (j > 0) {
        buff[j] = '\0';
        count++;
        if (count == 1) strcpy(word1, buff);
        else if (count == 2) strcpy(word2, buff);
        else if (count == 3) strcpy(word3, buff);
    }

    switch (count) {
        case 1:
            strcpy(opcode, word1);
            break;
        case 2:
            strcpy(opcode, word2);
            strcpy(operand, word1);
            break;
        case 3:
            strcpy(label, word3);
            strcpy(opcode, word2);
            strcpy(operand, word1);
            break;
    }
}

void PASS1() {
    printf("LOCATION LABEL\tOPERAND\tOPCODE\n");
    printf("_____________________________________\n");

    readLine(assemblyCode[0]);

    if (!strcmp(opcode, "START")) {
        startAddress = atoi(operand);
        locCounter = startAddress;
        strcpy(programName, label);
        printf("\n%04X\t%s\t%s\t%s", locCounter, label, opcode, operand);
    } else {
        programName[0] = '\0';
        startAddress = 0;
        locCounter = 0;
    }

    for (int i = 1; i < assemblyCodeSize; i++) {
        strcpy(line, assemblyCode[i]);
        readLine(line);
        if (!strcmp(opcode, "END")) break;

        printf("\n%04X\t%s\t%s\t%s", locCounter, label, opcode, operand);
        if (label[0] != '\0') checkLabel();
        checkOpCode();
    }

    printf("\n%04X\t\t%s\n", locCounter, "END");
}

void PASS2() {
    char record[30] = "", part[10] = "", value[10] = "";
    int currtxtlen = 0, recaddr = startAddress;

    printf("\n\nObject Code\n");
    printf("H^ %s ^ %04X ^ %04X\n", programName, startAddress, length);

    for (int i = 1; i < assemblyCodeSize; i++) {
        int operandaddr = 0, foundopcode = 0, foundoperand = 0;
        part[0] = value[0] = '\0';
        strcpy(line, assemblyCode[i]);
        readLine(line);

        if (!strcmp(opcode, "END")) break;

        for (int k = 0; k < 3; k++) {
            if (!strcmp(opcode, myOpT[k].code)) {
                foundopcode = 1;
                strcpy(part, myOpT[k].objcode);
                if (operand[0] != '\0') {
                    for (int chk = 0; chk < symCount; chk++) {
                        if (!strcmp(mySymTab[chk].symbol, operand)) {
                            sprintf(value, "%04X", mySymTab[chk].addr);
                            strcat(part, value);
                            foundoperand = 1;
                            break;
                        }
                    }
                    if (!foundoperand) strcat(part, "0000");
                }
                break;
            }
        }

        if (!foundopcode) {
            if (!strcmp(opcode, "BYTE") || !strcmp(opcode, "WORD"))
                strcpy(part, operand);
        }

        if ((currtxtlen + strlen(part)) <= 8) {
            strcat(record, "^");
            strcat(record, part);
            currtxtlen += strlen(part);
        } else {
            printf("T^ %04X ^%02X %s\n", recaddr, currtxtlen, record);
            recaddr += currtxtlen;
            currtxtlen = strlen(part);
            strcpy(record, part);
        }
    }

    if (currtxtlen > 0) {
        printf("T^ %04X ^%02X %s\n", recaddr, currtxtlen, record);
    }

    printf("E^ %04X\n", startAddress);
}

int main() {
    PASS1();
    length = locCounter - startAddress;
    PASS2();
    return 0;
}
