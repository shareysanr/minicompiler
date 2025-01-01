#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include "token.h"

typedef struct {
    const char* input;
    size_t index;
    char current_char;
} Lexer;

Lexer* init_lexer(const char* input);

Token get_next_token(Lexer* lexer);

TokenNode* applyLexer(Lexer* lexer);

void free_tokens(TokenNode* head);

void free_lexer(Lexer* lexer);

#endif
