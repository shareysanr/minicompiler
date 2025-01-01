#include <stdlib.h>
#include <stdio.h>
#include "lexer.h"

Lexer* init_lexer(const char* input) {
    Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
    if (!lexer){
        fprintf(stderr, "Error: Memory allocation failed for Lexer\n");
        exit(EXIT_FAILURE);
    }
    lexer->input = input;
    lexer->index = 0;
    lexer->current_char = input[0];
    return lexer;
}

void iterate(Lexer* lexer) {
    lexer->index++;
    lexer->current_char = lexer->input[lexer->index];
}

void skip_whitespace(Lexer* lexer) {
    while (is_space(lexer->current_char)) {
        iterate(lexer);
    }
}

int is_space(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int is_digit(char c) {
    return (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || 
            c == '5' || c == '6' || c == '7' || c == '8' || c == '9');
}

int getNum(Lexer* lexer) {
    int result = 0;
    int temp;
    while (is_digit(lexer->current_char)) {
        temp = lexer->current_char - '0';
        result = result * 10 + temp;
        iterate(lexer);
    }
    return result;
}

Token get_next_token(Lexer* lexer) {
    Token token;
    char cur = lexer->current_char;
    
    if (is_space(cur)){
        skip_whitespace(lexer);
    }

    if (is_digit(cur)){
        int num = getNum(lexer);
        token.type = TOKEN_NUMBER;
        token.value = num;
        return token;
    } else if (cur == '+'){
        token.type = TOKEN_PLUS;
    } else if (cur == '-'){
        token.type = TOKEN_MIN;
    } else if (cur == '*'){
        token.type = TOKEN_MUL;
    } else if (cur == '/'){
        token.type = TOKEN_DIV;
    } else if (cur == '('){
        token.type = TOKEN_L_PAREN;
    } else if (cur == ')'){
        token.type = TOKEN_R_PAREN;
    } else if (cur == '\0'){
        token.type = TOKEN_EOF;
    } else {
        fprintf(stderr, "Error: Unknown character '%c'\n", cur);
        exit(EXIT_FAILURE);
    }
    
    token.value = 0; // Value for non number tokens
    iterate(lexer);
    return token;
}

TokenNode* applyLexer(Lexer* lexer){
    TokenNode* tokensHead = (TokenNode*) malloc(sizeof(TokenNode));
    if (!tokensHead){
        fprintf(stderr, "Error: Memory allocation failed for tokensHead\n");
        exit(EXIT_FAILURE);
    }
    TokenNode* current = tokensHead;
    TokenNode* nextToken;
    Token tok;

    while (lexer->current_char != '\0'){
        tok = get_next_token(lexer);
        current->token = tok;
        nextToken = (TokenNode*) malloc(sizeof(TokenNode));
        if (!nextToken){
            fprintf(stderr, "Error: Memory allocation failed for nextToken\n");
            exit(EXIT_FAILURE);
        }
        current->next = nextToken;
        current = nextToken;
    }

    if (lexer->current_char == '\0') {
        tok = get_next_token(lexer); // Retrieves EOF token
        current->token = tok;
    }
    
    current->next = NULL;

    return tokensHead;
}

void free_tokens(TokenNode* head) {
    TokenNode* current = head;
    while (current != NULL) {
        TokenNode* next = current->next;
        free(current);
        current = next;
    }
}

void free_lexer(Lexer* lexer){
    free(lexer);
}