#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

int is_space(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int is_digit(char c) {
    return (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || 
            c == '5' || c == '6' || c == '7' || c == '8' || c == '9');
}

int is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_alphanumeric(char c) {
    return is_letter(c) || is_digit(c);
}

int is_int_keyword(Lexer* lexer) {
    // Make sure there are at least 3 characters left in the input
    if (lexer->index + 3 > strlen(lexer->input)) {
        return 0;
    }

    return (lexer->current_char == 'i' && lexer->input[lexer->index + 1] == 'n' &&
            lexer->input[lexer->index + 2] == 't' && (is_space(lexer->input[lexer->index + 3]) || 
            lexer->input[lexer->index + 3] == '\0' || lexer->input[lexer->index + 3] == ';' || 
            lexer->input[lexer->index + 3] == '='));
}

int is_if_keyword(Lexer* lexer) {
    // Make sure there are at least 2 characters left in the input
    if (lexer->index + 2 > strlen(lexer->input)) {
        return 0;
    }

    return (lexer->current_char == 'i' && lexer->input[lexer->index + 1] == 'f' && 
            (!is_alphanumeric(lexer->input[lexer->index + 2])));
}

int is_else_keyword(Lexer* lexer) {
    // Make sure there are at least 4 characters left in the input
    if (lexer->index + 4 > strlen(lexer->input)) {
        return 0;
    }

    return (lexer->current_char == 'e' && lexer->input[lexer->index + 1] == 'l' && 
            lexer->input[lexer->index + 2] == 's' && lexer->input[lexer->index + 3] == 'e' &&
            (!is_alphanumeric(lexer->input[lexer->index + 4])));
}

void skip_whitespace(Lexer* lexer) {
    while (lexer->current_char != '\0' && is_space(lexer->current_char)) {
        iterate(lexer);
    }
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
    
    if (is_digit(cur)){
        int num = getNum(lexer);
        token.type = TOKEN_NUMBER;
        token.value = num;
        return token;
    } else if (is_int_keyword(lexer)) {
        token.type = TOKEN_INT;
        token.value = 0;
        for (int i = 0; i < 3; i++) {
            iterate(lexer);
        }
        return token;
    } else if (is_if_keyword(lexer)) {
        token.type = TOKEN_IF;
        token.value = 0;
        for (int i = 0; i < 2; i++) {
            iterate(lexer);
        }
        return token;
    } else if (is_else_keyword(lexer)) {
        token.type = TOKEN_ELSE;
        token.value = 0;
        for (int i = 0; i < 4; i++) {
            iterate(lexer);
        }
        return token;
    } else if (is_letter(cur)) {
        char varName[32];  // compilers only value the first 31 characters of a variable name
        int length = 0;
        token.type = TOKEN_IDENTIFIER;

        while (is_alphanumeric(lexer->current_char) && length < 31) {
            varName[length] = lexer->current_char;
            length++;
            iterate(lexer);
        }
        varName[length] = '\0';

        strncpy(token.str, varName, sizeof(token.str));
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
    } else if (cur == '{'){
        token.type = TOKEN_L_BRACE;
    } else if (cur == '}'){
        token.type = TOKEN_R_BRACE;
    } else if (cur == '='){
        token.type = TOKEN_EQUALS;
    } else if (cur == ';'){
        token.type = TOKEN_SEMICOLON;
    } else if (cur == '\0'){
        token.type = TOKEN_EOF;
        token.value = 0;
        return token; // Don't want the lexer to iterate here so return
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

    skip_whitespace(lexer); // Skip whitespace once at the start of input

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

        skip_whitespace(lexer); // Skip whitespace once at the end of each token
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