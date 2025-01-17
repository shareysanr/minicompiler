#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include "token.h"

typedef struct Lexer{
    const char* input;
    size_t index;
    char current_char;
} Lexer;

Lexer* init_lexer(const char* input);

TokenNode* applyLexer(Lexer* lexer);

void free_tokens(TokenNode* head);

void free_lexer(Lexer* lexer);

#endif
