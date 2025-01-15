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
    TOKEN_L_BRACE,
    TOKEN_R_BRACE,
    TOKEN_IDENTIFIER,
    TOKEN_INT,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_IF_ELSE,
    TOKEN_EQUALS,
    TOKEN_SEMICOLON,
    TOKEN_STATEMENTS,
    TOKEN_EOF
} TokenType;

typedef struct Token{
    TokenType type;
    int value;
    char str[32];
} Token;

typedef struct TokenNode {
    Token token;
    struct TokenNode* next;
} TokenNode;

extern const char* token_names[];

#endif