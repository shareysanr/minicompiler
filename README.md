# QuickC: A C-Based Interpreter

QuickC is an interpreter built in C, designed for parsing and evaluating arithmetic expressions, handling variables, and supporting conditional logic. It starts with a given input where the lexer, parser, and interpreter are applied.
The lexer converts the input into tokens which serves as input for the parser. The parser then builds an Abstract Syntax Tree from the tokens for the interpreter to use. Finally, the interpreter takes the root node of the Abstract Syntax Tree and evaluates the tree nodes, effectively running the code. The variable values are shown at the end, reflecting the changes made through the code.

Optimizations were made to the parser in order to allow the interpreter to run faster. An Abstract Syntax Tree can have a lot of unnecessary nodes that can be eliminated so the interpreter can do its task quickly. Two optimizations that were introduced were constant folding and dead code elimination. In constant folding, expressions with constants are reduced to a single constant. For example, given the expression (x + 1 + 2 + 3 + 4), instead of 4 operator nodes, 4 constant integer nodes, and 1 variable node, the expression can be reduced to x + 10, eliminating 6 nodes from the Abstract Syntax Tree. In dead code elimination, statements that are impossible to be executed are removed from the Abstract Syntax Tree. For example, an if else statement with a condition of 3 will always be true, so the statements in the else block will never run. Thus, it is efficient to replace the if else subtree with the statements from the if block.

There also exists a REPL (Read Evaluate Print Loop) that allows the user to input a string of code. The input is then lexed, parsed, and interpreted. The user can input statements or expressions with variables, integers, and conditionals and observe the output. The REPL also has a feature that allows the user to see the effects of the optimizations by outputting the Abstract Syntax Tree before and after the optimization happens.

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
There are many features that I plan to implement in the future for this interpreter. As of right now, if statements run on expressions that are evaluated as integers, and are true/false depending on whether the integer is zero or not. I plan to implement logical operators such as && and ||, as well as comparison operators such as <,>, or ==. I plan to add loops for repetitive statement execution which include for loops and while loops. Multiple types will also be introduced, so integers aren't the only type. Function calls are also a significant part of programming that isn't yet in my interpreter, so that will also be a feature to be implemented.
