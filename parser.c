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

/*
Structure of if else in the AST:
The if_else node will be the root node of the if else expression. The left node will be the if
node, and the right node will be the else node. The left node of the if node will be the expression
to be evaluated, and the right node of the if node will be the statements to be executed if the
expression is true. The left node of the else statement will be the statements to be executed if
the expression is false.
                           if_else
            if                                  else
expression      if_statements   else_statements      NULL
*/
ASTNode* parse_if_else(TokenNode** current) {
    if ((*current) == NULL) {
        fprintf(stderr, "Error: NULL value for parse_if_else\n");
        exit(EXIT_FAILURE);
    }
    if ((*current)->token.type != TOKEN_IF && (*current)->next->token.type != TOKEN_L_PAREN) {
        fprintf(stderr, "Error: IF and ( tokens not found\n");
        exit(EXIT_FAILURE);
    }
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->token.type = TOKEN_IF_ELSE;

    ASTNode* if_node = (ASTNode*)malloc(sizeof(ASTNode));
    if_node->token.type = TOKEN_IF;
    ASTNode* else_node = (ASTNode*)malloc(sizeof(ASTNode));
    else_node->token.type = TOKEN_ELSE;
    node->left = if_node;
    node->right = else_node;

    *current = (*current)->next; // Move past if
    *current = (*current)->next; // Move past (
    if_node->left = parse_expression(current); 

    if ((*current)->token.type != TOKEN_R_PAREN && (*current)->next
        && (*current)->next->token.type != TOKEN_L_BRACE) {
        fprintf(stderr, "Error: ) and { tokens not found\n");
        exit(EXIT_FAILURE);
    }

    *current = (*current)->next; // Move past )
    *current = (*current)->next; // Move past {

    if_node->right = parse_statements(current);

    if ((*current) == NULL || (*current)->token.type != TOKEN_R_BRACE) {
        fprintf(stderr, "Error: NULL or R_BRACE not found in if\n");
        exit(EXIT_FAILURE);
    }

    *current = (*current)->next; // Move past }

    if ((*current)->token.type == TOKEN_ELSE) {
        *current = (*current)->next; // Move past else
        if ((*current)->token.type != TOKEN_L_BRACE) {
            fprintf(stderr, "Error: L_BRACE not found in else\n");
            exit(EXIT_FAILURE);
        }
        *current = (*current)->next; // Move past {
        else_node->left = parse_statements(current);
        else_node->right = NULL;

        if ((*current) == NULL || (*current)->token.type != TOKEN_R_BRACE) {
            fprintf(stderr, "Error: NULL or R_BRACE not found in else\n");
            exit(EXIT_FAILURE);
        }
        *current = (*current)->next; // Move past }
    }

    return node;
}

ASTNode* parse_statements(TokenNode** current) {
    if ((*current) == NULL) {
        fprintf(stderr, "Error: NULL value for parse_statements\n");
        exit(EXIT_FAILURE);
    }

    if ((*current)->token.type == TOKEN_R_BRACE || (*current)->token.type == TOKEN_EOF) {
        return NULL;
    }
    ASTNode* node;
    if ((*current)->token.type == TOKEN_IF) {
        //printf("HELLO\n");
        node = parse_if_else(current);
    } else {
        node = parse_assignment(current);
    }

    if (*current != NULL && (*current)->token.type != TOKEN_EOF
        && (*current)->token.type != TOKEN_R_BRACE) {
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

    if ((*current)->token.type == TOKEN_INT || ((*current)->token.type == TOKEN_IDENTIFIER
        && (*current)->next && (*current)->next->token.type == TOKEN_EQUALS) 
        || (*current)->token.type == TOKEN_IF) {
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

void constant_folding(ASTNode* node) {
    if (node == NULL) {
        return;
    }

    constant_folding(node->left);
    constant_folding(node->right);

    if ((node->token.type == TOKEN_PLUS || node->token.type == TOKEN_MIN ||
         node->token.type == TOKEN_MUL || node->token.type == TOKEN_DIV) &&
        node->left && node->right && node->left->token.type == TOKEN_NUMBER &&
        node->right->token.type == TOKEN_NUMBER) {
            
        int left_val = node->left->token.value;
        int right_val = node->right->token.value;
        int result;

        if (node->token.type == TOKEN_PLUS) {
            result = left_val + right_val;
        } else if (node->token.type == TOKEN_MIN) {
            result = left_val - right_val;
        } else if (node->token.type == TOKEN_MUL) {
            result = left_val * right_val;
        } else if (node->token.type == TOKEN_DIV) {
            if (right_val == 0) {
                fprintf(stderr, "Error: Dividing by zero in constant folding\n");
                exit(EXIT_FAILURE);
            }
            result = left_val / right_val;
        }

        node->token.type = TOKEN_NUMBER;
        node->token.value = result;

        // Free left and right nodes
        free_ast(node->left);
        free_ast(node->right);
        
        node->left = NULL;
        node->right = NULL;
    }
}

ASTNode* dead_code_elim(ASTNode* node) {
    if (node == NULL) {
        return NULL;
    }
    
    // Retrieve the node of the statements and replace the if_else node with it if applicable
    if (node->token.type == TOKEN_IF_ELSE && node->left->left->token.type == TOKEN_NUMBER) {
        if (node->left->left->token.value != 0) {
            ASTNode* smts = node->left->right;
            free_ast(node->right);
            free_ast(node->left->left);
            free(node->left);
            free(node);
            return smts;
        } else {
            ASTNode* smts = node->right->left;
            free_ast(node->left);
            free(node->right);
            free(node);
            return smts;
        }
    }

    node->left = dead_code_elim(node->left);
    node->right = dead_code_elim(node->right);
    return node;
}

void free_ast(ASTNode* root) {
    if (root == NULL) {
        return;
    }
    free_ast(root->left);
    free_ast(root->right);
    free(root);
}
