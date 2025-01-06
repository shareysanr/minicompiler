#ifndef PARSER_H
#define PARSER_H

#include "token.h"

typedef struct ASTNode {
    Token token;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

ASTNode* parse_tokens(TokenNode** tokenHead);

ASTNode* parse_expression(TokenNode** current);

ASTNode* parse_term(TokenNode** current);

ASTNode* parse_factor(TokenNode** current);

void free_ast(ASTNode* root);

#endif