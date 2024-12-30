#ifndef TOKENS_H
#define TOKENS_H


typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MIN,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_L_PAREN,
    TOKEN_R_PAREN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    int value;
} Token;

#endif