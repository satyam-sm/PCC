#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 10

struct identifier 
{
    char name[20];
    char type[20];
    char value[20];
    int line;
    struct identifier *next;
};
struct identifier *table[N] = {NULL};

int hash(char *name) 
{
    int i, sum = 0;
    for (i = 0; name[i] != '\0'; i++) 
    {
        sum += name[i];
    }
    return sum % N;
}

void insert(char *name, char *type, char *value, int line) 
{
    int index = hash(name);
    struct identifier *temp = (struct identifier *)malloc(sizeof(struct identifier));
    strcpy(temp->name, name);
    strcpy(temp->type, type);
    strcpy(temp->value, value);
    temp->line = line;
    temp->next = table[index];
    table[index] = temp;
}

struct identifier *search(char *name) 
{
    int index = hash(name);
    struct identifier *temp = table[index];
    while (temp != NULL) 
    {
        if (strcmp(temp->name, name) == 0) 
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void delete(char *name) 
{
    int index = hash(name);
    struct identifier *temp = table[index];
    struct identifier *prev = NULL;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) 
        {
            if (prev == NULL) 
            {
                table[index] = temp->next;
            } 
            else 
            {
                prev->next = temp->next;
            }
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

void print() 
{
    int i;
    for (i = 0; i < N; i++) 
    {
        printf("%d : ", i);
        struct identifier *temp = table[i];
        while (temp != NULL) 
        {
            printf("[%-5s %-7s %-10s %-2d] -> ", temp->name, temp->type, temp->value, temp->line);
            temp = temp->next;
        }
        printf("[NULL]\n\n");
    }
}

int main() 
{
    FILE *fp = fopen("Code.c", "r");
    char line[100];
    if (fp == NULL) 
    {
        printf("Error in opening file.\n");
        exit(1);
    }

    int lineNo = 0;
    while (fgets(line, 100, fp) != NULL) 
    {
        lineNo++;

        // Read line and check if it is a variable declaration
        if (strstr(line, "=") != NULL) 
        {
            // Split the line into tokens to parse the variable declaration
            char *type = strtok(line, " ");
            char *name = strtok(NULL, " ");
            char *check = strtok(NULL, " ");
            if (check != NULL && strcmp(check, "=") == 0) 
            {
                char *value = strtok(NULL, ";");
                if (value != NULL) 
                {
                    insert(name, type, value, lineNo);
                }
            }
        }
    }

    print();
    fclose(fp);
    return 0;
}


