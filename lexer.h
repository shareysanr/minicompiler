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

#endif
