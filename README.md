# QuickC: A C-Based Interpreter

QuickC is an interpreter built in C, designed for parsing and evaluating arithmetic expressions, handling variables, and supporting conditional logic. It starts with a given input where the lexer, parser, and interpreter are applied.
The lexer converts the input into tokens which serves as input for the parser. The parser then builds an Abstract Syntax Tree from the tokens for the interpreter to use. Finally, the interpreter takes the root node of the Abstract Syntax Tree and evaluates the tree nodes, effectively running the code. The variable values are shown at the end, reflecting the changes made through the code.

## Grammar
The following grammar was designed for the interpreter:

program ::= statements | expression  
statements ::= statement | statements statement  
statement ::= assignment | if_else  
if_else ::= "if" "(" expression ")" "{" statements "}" | "if" "(" expression ")" "{" statements "}" "else" "{" statements "}"  
assignment ::= "int" identifier ";" | "int" identifier "=" expression ";" | identifier "=" expression ";"  
expression ::= term | expression "+" term | expression "-" term  
term ::= factor | term "*" factor | term "/" factor  
factor ::= [0-9]+ | "(" expression ")" | "-" factor | identifier  
identifier ::= [a-zA-Z][a-zA-Z0-9]\*

## Getting Started
### Building the Project
1. Clone the repository:
   ```bash
   git clone https://github.com/shareysanr/quickc.git
   cd quickc
2. Compile the project:
   ```bash
   make
3. Run the executable:
   ```bash
   ./quickc


## Future Additions
I plan to add features for optimization and extra functionality. Optimizations include constant folding to reduce as many constants as possible, and dead code elimination to
get rid of code that is impossible to run. These optimizations reduce the number of nodes in the Abstract Syntax Tree, and allow interpretation to happen faster. Features for
extra functionality include implementing increment and decrement operators as well as function calls. These changes will allow the user to do more with the interpreter.
