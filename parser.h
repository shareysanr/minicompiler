#ifndef PARSER_H
#define PARSER_H

#include "token.h"

typedef struct ASTNode {
    Token token;
    struct ASTNode* left;
    struct ASTNode* right;
}

ASTNode* parse_tokens(TokenNode** tokenHead);





#endif