#include <stdio.h>
// Types for 
enum Type {
	MULT, ADD, INTEGER, TEXT,
	PAREN_LEFT, PAREN_RIGHT
};

struct Token {
	int type;
	int value; // Also length
	char string[20];
};

int lex(struct Token *reading, char string[], size_t *c);
int parse(struct Token *tokens, char string[]);
