CC = gcc

OUTPUT = program

SRCS = main.c lexer.c parser.c interpreter.c

all:
	$(CC) -o $(OUTPUT) $(SRCS)

clean:
	rm -f $(OUTPUT)

.PHONY: all clean
