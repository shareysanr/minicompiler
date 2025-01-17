#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

#define MAX_VARIABLES 100

typedef struct Variable{
    char name[32];
    int value;
} Variable;

void print_variable_table();

int interpret(ASTNode* node);

void interpret_program(ASTNode* node);


#endif
