#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

#define MAX_VARIABLES 100

typedef struct Variable{
    char name[32];
    int value;
} Variable;

void init_table();
int get_variable_index(const char* name);
int exists(const char* name);
int interpret(ASTNode* node);


#endif
