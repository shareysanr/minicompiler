#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

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

void print_tree(ASTNode* root, int level) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < level; i++) {
        printf("    ");
    }

    if (root->token.type == TOKEN_NUMBER) {
        printf("%d\n", root->token.value);
    } else {
        printf("%s\n", token_signs[root->token.type]);
    }

    print_tree(root->left, level + 1);
    print_tree(root->right, level + 1);
}

void compile_multiple(const char* input_list[]) {
    for (int i = 0; input_list[i] != NULL; i++) {
        printf("Input string: \"%s\"\n", input_list[i]);
        Lexer* lexer = init_lexer(input_list[i]);

        TokenNode* tokens = applyLexer(lexer);
        TokenNode* tokens_passed = tokens;

        printf("\n\nTokens:\n");
        print_tokens(tokens);

        ASTNode* ast_root = parse_tokens(&tokens_passed);

        printf("\n\nAbstract Syntax Tree:\n");
        //print_ast(ast_root);
        print_tree(ast_root, 0);

        int return_value = interpret(ast_root);

        printf("\n\nInterpret AST:\n");
        printf("Value = '%d'\n", return_value);
        printf("\n\n\n\n");

        free_ast(ast_root);
        free_tokens(tokens);
        free_lexer(lexer);
    }
}

int main() {
    const char* input_list[] = {"  3 + 4 * 2 ", "  10 - 2 - 3 ", " 6  / (2 + 1)  ",
                                " ((3 + 2) * 4) - (6 / 2)", "((2 + 3) * 4 - 5) / (2 + 3)", NULL};

    compile_multiple(input_list);

    /*
    const char* input = "123 + 456 * (7 - 8)";
    //const char* input = "1 + (";

    printf("Input string: \"%s\"\n", input);
    Lexer* lexer = init_lexer(input);

    TokenNode* tokens = applyLexer(lexer);
    TokenNode* tokens_passed = tokens;

    printf("\n\nTokens:\n");
    print_tokens(tokens);

    ASTNode* ast_root = parse_tokens(&tokens_passed);

    printf("\n\nAbstract Syntax Tree:\n");
    //print_ast(ast_root);
    print_tree(ast_root, 0);

    int return_value = interpret(ast_root);

    printf("\n\nInterpret AST:\n");
    printf("Value ='%d'\n", return_value);
    
    free_ast(ast_root);
    free_tokens(tokens);
    free_lexer(lexer);
    */

    return 0;
}