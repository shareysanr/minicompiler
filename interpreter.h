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
int get_variable_count();
Variable* get_variable_table();
void print_variable_table();
int exists(const char* name);
int interpret(ASTNode* node);
void interpret_program(ASTNode* node);


#endif
