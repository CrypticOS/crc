#include <stdio.h>

enum Type {
	MULT, ADD, INTEGER, TEXT,
	PAREN_LEFT, PAREN_RIGHT
};

struct Token {
	int type;
	int value; // Also length
	char string[20];
};

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
		break;
	case '*':
		reading->type = MULT; (*c)++;
		break;
	case '(':
		reading->type = PAREN_LEFT; (*c)++;
		break;
	case ')':
		reading->type = PAREN_RIGHT; (*c)++;
		break;
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

int main() {
	puts("inc \"$kernel/main.casm\"");
	puts("jmp top\ninc \"$kernel/math/add.casm\"\ntop:");
	
	char string[] = "(1 + 1)";
	size_t c = 0;
	struct Token reading;

	// Operator 
	struct Token operatorStack[10];
	size_t operator = 0;

	// Output
	struct Token tokens[20];
	size_t token = 0;

	// Now, the shunting yard algorithm.
	// Designed by Edsger Dijkstra in 1961.
	while (!lex(&reading, string, &c)) {
		if (reading.type == INTEGER) {
			tokens[token] = reading;
			token++;
		} else if (reading.type == PAREN_LEFT) {
			operatorStack[operator].type = PAREN_LEFT;
			operator++;
		} else if (reading.type == PAREN_RIGHT) {
			operator--;
			while (operatorStack[operator].type != PAREN_LEFT) {
				tokens[token].type = operatorStack[operator].type;
				token++;
				operator--;
			}

			// Discard left parenthesis
			if (operatorStack[operator].type == PAREN_LEFT) {
				operator--;
			} else {
				// TODO: Mismatched parenthesis
			}

			operator++; // Move to unused
		} else if (reading.type == PAREN_LEFT) {
			tokens[token].type = operatorStack[operator].type;
			token++;
			operator--;
		} else {
			// Else, it is an operator.
	
			// Push remaining operators to output
			while (operator != 0 && tokens[token].type != '(') {
				operator--;
				tokens[token].type = operatorStack[operator].type;
				token++;
			}
	
			operatorStack[operator].type = reading.type;
			operator++; // Move to unused
		}	
	}

	// Push the remaining operators to the output
	while (operator != 0) {
		operator--;
		tokens[token].type = operatorStack[operator].type;
		token++;
	}

	char *regs[] = {"ga", "gb", "gc", "gd"};
	size_t currentReg = 0;
	
	for (size_t i = 0; i < token; i++) {
		switch (tokens[i].type) {
		case INTEGER:
			printf("set %s %d\n", regs[currentReg], tokens[i].value);
			currentReg++;
			
			break;
		case ADD:
			puts("run k.add");
			currentReg = 0;
			printf("set %s gr\n", regs[currentReg]);
			currentReg++;
			break;
		}
	}

	puts("add ga '0'\nprt ga");
}
