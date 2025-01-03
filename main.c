#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

void print_tokens(TokenNode* head) {
    TokenNode* current = head;
    while (current != NULL) {
        printf("Token: type=%d", current->token.type);
        if (current->token.type == TOKEN_NUMBER) {
            printf(", value=%d", current->token.value);
        }
        printf("\n");
        current = current->next;
    }
}

int main() {
    const char* input = "123 + 456 * (7 - 8)";

    Lexer* lexer = init_lexer(input);

    TokenNode* tokens = applyLexer(lexer);

    printf("Tokens:\n");
    print_tokens(tokens);

    free_tokens(tokens);
    free_lexer(lexer);

    return 0;
}