#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "parser.h"



ASTNode* parse_factor(TokenNode** current) {
    if ((*current) == NULL) {
        fprintf(stderr, "Error: NULL value for parse_factor\n");
        exit(EXIT_FAILURE);
    }

    if ((*current)->token.type == TOKEN_NUMBER) {
        ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
        node->token = (*current)->token;
        node->left = NULL;
        node->right = NULL;

        *current = (*current)->next;
        return node;
    }

    if ((*current)->token.type == TOKEN_L_PAREN) {
        *current = (*current)->next;
        ASTNode* node = parse_expression(current);

        if ((*current)->token.type != TOKEN_R_PAREN) {
            fprintf(stderr, "Error: Expected right parenthesis\n");
            exit(EXIT_FAILURE);
        }

        *current = (*current)->next;
        return node;
    }

    if ((*current)->token.type == TOKEN_MIN) {
        ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
        node->token = (*current)->token;
        *current = (*current)->next;

        node->left = parse_factor(current); // Set the left node to the number or expression
        node->right = NULL;
        return node;
    }

    if ((*current)->token.type == TOKEN_IDENTIFIER) {
        ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
        node->token = (*current)->token;
        *current = (*current)->next;

        node->left = NULL;
        node->right = NULL;
        return node;
    }

    fprintf(stderr, "Error: Unexpected token '%s' in parse_factor\n", token_names[(*current)->token.type]);
    exit(EXIT_FAILURE);
}

ASTNode* parse_term(TokenNode** current) {
    if ((*current) == NULL) {
        fprintf(stderr, "Error: NULL value for parse_term\n");
        exit(EXIT_FAILURE);
    }

    ASTNode* node = parse_factor(current);

    while ((*current) != NULL && ((*current)->token.type == TOKEN_MUL || (*current)->token.type == TOKEN_DIV)) {
        ASTNode* new_node = (ASTNode*)malloc(sizeof(ASTNode));
        new_node->token = (*current)->token;
        new_node->left = node;
        *current = (*current)->next;
        new_node->right = parse_factor(current);
        node = new_node;
    }
    
    return node;
}

ASTNode* parse_expression(TokenNode** current){
    ASTNode* node = parse_term(current);

    while ((*current) != NULL && ((*current)->token.type == TOKEN_PLUS || (*current)->token.type == TOKEN_MIN)) {
        ASTNode* new_node = (ASTNode*)malloc(sizeof(ASTNode));

        if ((*current)->next == NULL || (*current)->next->token.type == TOKEN_EOF) {
            fprintf(stderr, "Error: Incomplete expression after '%s'\n",
                    token_names[(*current)->token.type]);
            exit(EXIT_FAILURE);
        }

        new_node->token = (*current)->token;
        new_node->left = node;
        *current = (*current)->next;
        new_node->right = parse_term(current);
        node = new_node;
    }
    
    return node;
}

ASTNode* parse_assignment(TokenNode** current) {
    if ((*current) == NULL) {
        fprintf(stderr, "Error: NULL value for parse_term\n");
        exit(EXIT_FAILURE);
    }

    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->left = NULL;
    node->right = NULL;
    // int x; and int x = 5; cases
    if ((*current)->token.type == TOKEN_INT) {
        *current = (*current)->next;
        if ((*current)->token.type != TOKEN_IDENTIFIER) {
            fprintf(stderr, "Error: Variable not after keyword int\n");
            exit(EXIT_FAILURE);
        }

        node->token = (*current)->token;
        *current = (*current)->next;

        if ((*current)->token.type == TOKEN_SEMICOLON) {
            node->left = NULL;
            node->right = NULL;
            *current = (*current)->next;
            return node;
        } else if ((*current)->token.type == TOKEN_EQUALS) {
            *current = (*current)->next;
            node->left = parse_expression(current);
            node->right = NULL;

            if ((*current)->token.type != TOKEN_SEMICOLON) {
                fprintf(stderr, "Error: Semicolon not found after expression\n");
                exit(EXIT_FAILURE);
            }

            *current = (*current)->next;
            return node;
        } else {
            fprintf(stderr, "Error: Incorrect token found after int \n");
            exit(EXIT_FAILURE);
        }
    } else if ((*current)->token.type == TOKEN_IDENTIFIER) {
        // x = 5; case
        node->token = (*current)->token;
        *current = (*current)->next;

        if ((*current)->token.type == TOKEN_EQUALS) {
            *current = (*current)->next;
            
            node->left = parse_expression(current);
            node->right = NULL;

            if ((*current)->token.type != TOKEN_SEMICOLON) {
                fprintf(stderr, "Error: Semicolon not found after expression\n");
                exit(EXIT_FAILURE);
            }

            *current = (*current)->next;
            return node;
        } else {
            fprintf(stderr, "Error: Incorrect token found after identifier\n");
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Error: Incorrect match for assignment\n");
        exit(EXIT_FAILURE);
    }
}

ASTNode* parse_statements(TokenNode** current) {
    if ((*current) == NULL) {
        fprintf(stderr, "Error: NULL value for parse_statements\n");
        exit(EXIT_FAILURE);
    }

    ASTNode* node = parse_assignment(current);

    if (*current != NULL && (*current)->token.type != TOKEN_EOF) {
        ASTNode* new_node = (ASTNode*)malloc(sizeof(ASTNode));
        new_node->token.type = TOKEN_STATEMENTS;
        new_node->left = node;
        new_node->right = parse_statements(current);
        return new_node;
    }
    
    return node;
}

ASTNode* parse_program(TokenNode** current) {
    if ((*current) == NULL) {
        fprintf(stderr, "Error: NULL value for parse_program\n");
        exit(EXIT_FAILURE);
    }

    if ((*current)->token.type == TOKEN_INT || (*current)->token.type == TOKEN_IDENTIFIER) {
        return parse_statements(current);
    } else {
        return parse_expression(current);
    }
}

ASTNode* parse_tokens(TokenNode** tokenHead) {
    if (*tokenHead == NULL) {
        fprintf(stderr, "Error: Token list is empty\n");
        exit(EXIT_FAILURE);
    }

    ASTNode* root = parse_program(tokenHead);

    if (*tokenHead == NULL || (*tokenHead)->token.type != TOKEN_EOF) {
        fprintf(stderr, "Error: Unexpected token after end of expression\n");
        exit(EXIT_FAILURE);
    }

    return root;
}

void free_ast(ASTNode* root) {
    if (root == NULL) {
        return;
    }
    free_ast(root->left);
    free_ast(root->right);
    free(root);
}
