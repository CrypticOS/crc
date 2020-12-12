#include "header.h"

// Main lexer. Lexes a string from a given point.
int lex(struct Token *reading, char string[], size_t *c) {
	while (string[*c] == ' ') {
		(*c)++;
	}

	// EOF
	if (string[*c] == '\0') {
		return -1;
	}
	
	reading->type = 0;
	reading->value = 0;

	switch (string[*c]) {
	case '+':
		reading->type = ADD; (*c)++;
		return 0;
	case '*':
		reading->type = MULT; (*c)++;
		return 0;
	case '(':
		reading->type = PAREN_LEFT; (*c)++;
		return 0;
	case ')':
		reading->type = PAREN_RIGHT; (*c)++;
		return 0;
	}

	// Lex numbers
	while (string[*c] >= '0' && string[*c] <= '9') {
		reading->type = INTEGER;
		reading->value *= 10;
		reading->value += string[*c] - '0';
		(*c)++;
	}

	// Lex text (function names)
	while ((string[*c] >= 'a' && string[*c] <= 'z') || (string[*c] >= 'A' && string[*c] <= 'Z')) {
		reading->type = TEXT;
		reading->string[reading->value] = string[*c];
		reading->value++;
		(*c)++;
	}

	// Lex strings
	if (string[*c] == '\"') {
		(*c)++;
		while (string[*c] != '\"') {
			reading->type = TEXT;
			reading->string[reading->value] = string[*c];
			reading->value++;
		}
	}

	// Return OK
	return 0;
}