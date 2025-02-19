#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "parser.h"
#include "interpreter.h"

Variable variable_table[MAX_VARIABLES];
int variable_count = 0;

void init_table() {
    variable_count = 0;
}

int get_variable_index(const char* name) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variable_table[i].name, name) == 0) {
            return i;
        }
    }
    
    if (variable_count >= MAX_VARIABLES) {
        fprintf(stderr, "Error: Symbol table full\n");
        exit(EXIT_FAILURE);
    }

    // If variable is not found
    strcpy(variable_table[variable_count].name, name);
    variable_table[variable_count].value = 0;
    variable_count++;
    return variable_count - 1;
}

int get_variable_count() {
    return variable_count;
}

Variable* get_variable_table() {
    return variable_table;
}

void print_variable_table() {
    printf("Variable Table:\n");
    for (int i = 0; i < variable_count; i++) {
        printf("%s = %d\n", variable_table[i].name, variable_table[i].value);
    }
}

int exists(const char* name) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variable_table[i].name, name) == 0) {
            return 1;
        }
    }
    return 0;
}

int interpret(ASTNode* node) {
    if (node == NULL) {
        fprintf(stderr, "Error: Null AST node\n");
        exit(EXIT_FAILURE);
    }

    if (node->token.type == TOKEN_NUMBER) {
        return node->token.value;
    } else if (node->token.type == TOKEN_IDENTIFIER) {
        int variable_exists = exists(node->token.str);
        int index = get_variable_index(node->token.str);
        if (node->left != NULL) {
            // Case where variable needs to be set to a value
            int value = interpret(node->left);
            variable_table[index].value = value;
            //return value;
            return -99999;
        } else {
            if (!variable_exists) {
                variable_table[index].value = 0; // Case of initializion with no assignment ("int x;")
                return -99999;
            }
            // Case where variable is already initialized and is on RHS for substitution
            return variable_table[index].value;
        }
    } else if (node->token.type == TOKEN_PLUS) {
        return interpret(node->left) + interpret(node->right);
    } else if (node->token.type == TOKEN_MIN) {
        if (node->right == NULL) {
            return -1 * interpret(node->left);
        }
        return interpret(node->left) - interpret(node->right);
    } else if (node->token.type == TOKEN_MUL) {
        return interpret(node->left) * interpret(node->right);
    } else if (node->token.type == TOKEN_DIV) {
        int right_value = interpret(node->right);
        if (right_value == 0) {
            fprintf(stderr, "Error: Division by zero\n");
            exit(EXIT_FAILURE);
        }
        return interpret(node->left) / right_value;
    } else if (node->token.type == TOKEN_IF_ELSE) {
        int condition = interpret(node->left->left);
        if (condition) {
            interpret_program(node->left->right);
        } else if (node->right->left) {
            interpret_program(node->right->left);
        }
        return -99999;
    }

    fprintf(stderr, "Error: Unknown token type '%s'\n", token_names[node->token.type]);
    exit(EXIT_FAILURE);
}

void interpret_program(ASTNode* node) {
    if (node == NULL) {
        return;
    }
    
    if (node->token.type == TOKEN_STATEMENTS) {
        interpret(node->left);
        interpret_program(node->right);
    } else {
        interpret(node);
    }
}