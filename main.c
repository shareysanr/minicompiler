#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    "TOKEN_L_BRACE",
    "TOKEN_R_BRACE",
    "TOKEN_IDENTIFIER",
    "TOKEN_INT",
    "TOKEN_IF",
    "TOKEN_ELSE",
    "TOKEN_IF_ELSE",
    "TOKEN_EQUALS",
    "TOKEN_SEMICOLON",
    "TOKEN_STATEMENTS",
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
    "{",
    "}",
    "VAR",
    "INT",
    "IF",
    "ELSE",
    "IF_ELSE",
    "=",
    ";",
    "SMTS",
    "EOF"
};

void print_tokens(TokenNode* head) {
    TokenNode* current = head;
    while (current != NULL) {
        printf("Token: type=%s", token_names[current->token.type]);
        if (current->token.type == TOKEN_NUMBER) {
            printf(", value=%d", current->token.value);
        } else if (current->token.type == TOKEN_IDENTIFIER) {
            printf(", name=%s", current->token.str);
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
    } else if (root->token.type == TOKEN_IDENTIFIER) {
        printf("V %s\n", root->token.str);
    } else {
        printf("%s\n", token_signs[root->token.type]);
    }

    print_tree(root->left, level + 1);
    print_tree(root->right, level + 1);
}

void lex_multiple(const char* input_list[]) {
    for (int i = 0; input_list[i] != NULL; i++) {
        printf("Input string: \"%s\"\n", input_list[i]);
        Lexer* lexer = init_lexer(input_list[i]);

        TokenNode* tokens = applyLexer(lexer);
        TokenNode* tokens_passed = tokens;

        printf("\n\nTokens:\n");
        print_tokens(tokens);
        printf("\n\n\n\n");

        free_tokens(tokens);
        free_lexer(lexer);
    }
}

void parse_multiple(const char* input_list[]) {
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
        printf("\n\n\n\n");

        free_ast(ast_root);
        free_tokens(tokens);
        free_lexer(lexer);
    }
}

void interpret_multiple(const char* input_list[]) {
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

void interpret_block(const char* input) {
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

    //int return_value = interpret(ast_root);
    interpret_program(ast_root);

    printf("\n\nInterpret AST:\n");
    //printf("Value = '%d'\n", return_value);
    print_variable_table();
    printf("\n\n\n\n");

    free_ast(ast_root);
    free_tokens(tokens);
    free_lexer(lexer);
}

void repl() {
    char input[256];
    printf("QuickC REPL - Type 'exit' to quit\n");
    while (1) {
        printf(">>> ");
        if (!fgets(input, 256, stdin)) {
            break;;
        }
        size_t len = strlen(input);

        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        len = strlen(input);
        
        if (strcmp(input, "exit") == 0) {
            break;
        }
        if (strlen(input) == 0) {
            continue;
        }


        //printf("%s\n", input);

        Lexer* lexer = init_lexer(input);

        TokenNode* tokens = applyLexer(lexer);
        TokenNode* tokens_passed = tokens;

        ASTNode* ast_root = parse_tokens(&tokens_passed);

        int result = interpret(ast_root);
        if (result != -99999) {
            printf("%d\n", result);
        }
    }
}

int main() {
    //const char* input_list[] = {"  3 + 4 * 2 ", "  10 - 2 - 3 ", " 6  / (2 + 1)  ",
    //                            " ((3 + 2) * 4) - (6 / 2)", "((2 + 3) * 4 - 5) / (2 + 3)", NULL};

    const char* lex_list[] = {
        "if ( 3+ 5) { int x = x + 5;}",
        "else",
        "ifint;if(else)34;+if",
        "inty x y z;",
        "(-3 + 4)int",
        "1 + -3 * -2int",
        NULL
    };
    //lex_multiple(lex_list);

    const char* parse_list[] = {
        "if (3 + 5) {int x = x + 5; int y = y + 3;} else { int a = a + 1; int c = c + 2;} int z = z + 7;",
        //"int x;\n x = 5;\n x = x + 5;",
        //"int x = 5; x = 5 * 4 + 2;",
        //"int x = 4 + 5 * (9 - 2); x = x + 2;",
        NULL
    };
    //parse_multiple(parse_list);

    const char* interpret_list[] = {
        "if (3 + 5) {int x = x + 5; int y = y + 3;} else { int a = a + 1; int c = c + 2;} int z = z + 7;",
        NULL
    };
    //interpret_multiple(interpret_list); // Only expressions or one assignment


    const char* input = "if (5 + -5) {int x = x + 5; int y = y + 3;} else { int a = a + 1; int c = c + 2;} int z = z + 7;";
    //interpret_block(input);
    repl();

    //const char* input = "123 + 456 * (7 - 8)";
    /*
    const char* input = "int x = 5;";

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