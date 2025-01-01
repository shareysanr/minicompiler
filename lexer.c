#include <stdlib.h>
#include "lexer.h"

Lexer* init_lexer(const char* input) {
    Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
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
    while (lexer->current_char != '\0') {
        char cur = lexer->current_char;
        if (is_space(cur)) {
            skip_whitespace(lexer);
            continue;
        } else if (is_digit(cur)) {
            int num = getNum(lexer);
            token.type = TOKEN_NUMBER;
            token.value = num;
            return token;
        } else if (cur == '+') {
            token.type = TOKEN_PLUS;
            token.value = 0;
            iterate(lexer);
            return token;
        } else if (cur == '-') {
            token.type = TOKEN_MIN;
            token.value = 0;
            iterate(lexer);
            return token;
        } else if (cur == '*') {
            token.type = TOKEN_MUL;
            token.value = 0;
            iterate(lexer);
            return token;
        } else if (cur == '/') {
            token.type = TOKEN_DIV;
            token.value = 0;
            iterate(lexer);
            return token;
        } else if (cur == '(') {
            token.type = TOKEN_L_PAREN;
            token.value = 0;
            iterate(lexer);
            return token;
        } else if (cur == ')') {
            token.type = TOKEN_R_PAREN;
            token.value = 0;
            iterate(lexer);
            return token;
        } else {
            // Error reached
        }
    }
    token.type = TOKEN_EOF;
    return token;
}