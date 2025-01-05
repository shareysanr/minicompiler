#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "parser.h"

ASTNode* parse_tokens(TokenNode** tokenHead) {
    if (*tokenHead == NULL) {
        fprintf(stderr, "Error: Token list is empty\n");
        exit(EXIT_FAILURE);
    }

    ASTNode* root = parse_expression(tokenHead);

    if (*tokenHead == NULL || (*tokenHead)->token.type != TOKEN_EOF) {
        fprintf(stderr, "Error: Unexpected tokens after end of expression\n");
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
