#ifndef PARSER_H
#define PARSER_H

#include "token.h"

typedef struct ASTNode {
    Token token;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

ASTNode* parse_tokens(TokenNode** tokenHead);

void constant_folding(ASTNode* node);

ASTNode* dead_code_elim(ASTNode* node);

void free_ast(ASTNode* root);

#endif