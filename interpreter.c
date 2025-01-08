#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "parser.h"
#include "interpreter.h"

int interpret(ASTNode* node) {
    if (node == NULL) {
        fprintf(stderr, "Error: Null AST node\n");
        exit(EXIT_FAILURE);
    }

    if (node->token.type == TOKEN_NUMBER) {
        return node->token.value;
    } else if (node->token.type == TOKEN_PLUS) {
        return interpret(node->left) + interpret(node->right);
    } else if (node->token.type == TOKEN_MIN) {
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
    }

    fprintf(stderr, "Error: Unknown token type '%s'\n", token_names[node->token.type]);
    exit(EXIT_FAILURE);
}