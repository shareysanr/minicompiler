#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

const char* token_names[] = {
    "TOKEN_NUMBER",
    "TOKEN_PLUS",
    "TOKEN_MIN",
    "TOKEN_MUL",
    "TOKEN_DIV",
    "TOKEN_L_PAREN",
    "TOKEN_R_PAREN",
    "TOKEN_EOF"
};

const char* token_signs[] = {
    "",
    "+",
    "-",
    "*",
    "/",
    "(",
    ")",
    "EOF"
};

void print_tokens(TokenNode* head) {
    TokenNode* current = head;
    while (current != NULL) {
        printf("Token: type=%s", token_names[current->token.type]);
        if (current->token.type == TOKEN_NUMBER) {
            printf(", value=%d", current->token.value);
        }
        printf("\n");
        current = current->next;
    }
}

void print_ast(ASTNode* root) {
    if (root == NULL) {
        return;
    }
    printf("%s", token_names[root->token.type]);
    if (root->token.type == TOKEN_NUMBER) {
        printf(" %d", root->token.value);
    }
    printf("\n");
    print_ast(root->left);
    print_ast(root->right);
}

int main() {
    const char* input = "123 + 456 * (7 - 8)";
    //const char* input = "1 + (";

    printf("Input string: \"%s\"\n", input);
    Lexer* lexer = init_lexer(input);

    TokenNode* tokens = applyLexer(lexer);
    TokenNode* tokens_passed = tokens;

    printf("Tokens:\n");
    print_tokens(tokens);

    printf("\n\n");

    ASTNode* ast_root = parse_tokens(&tokens_passed);

    printf("Abstract Syntax Tree:\n\n");
    print_ast(ast_root);
    
    free_ast(ast_root);
    free_tokens(tokens);
    free_lexer(lexer);

    return 0;
}