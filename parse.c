#include "header.h"


int parse(struct Token *tokens, char string[]) {
	// Token read for 
	size_t c = 0;
	struct Token reading;

	// Operator stack
	struct Token operatorStack[10];
	int operator = 0;

	int token = 0;

	// Now, the shunting yard algorithm.
	// Designed by Edsger Dijkstra in 1961.
	while (!lex(&reading, string, &c)) {
		if (reading.type == INTEGER) {
			tokens[token] = reading;
			token++;
		} else if (reading.type == PAREN_LEFT) {
			operatorStack[operator] = reading;
			operator++;
		} else if (reading.type == PAREN_RIGHT) {
			operator--;
			while (operator != 0 && operatorStack[operator].type != PAREN_LEFT) {
				tokens[token].type = operatorStack[operator].type;
				token++;
				operator--;

				if (operator < 0) {
					puts("; ERR: Mismatched parenthesis.");
					return -1;
				}
			}

			// Discard left parenthesis
			if (operatorStack[operator].type == PAREN_LEFT) {
				operator--;
			} else {
				puts("; ERR: Mismatched parenthesis.");
				return -1;
			}

			operator++; // Move to unused
		} else {
			// Else, it is an operator. (+, *, /)
	
			// Push remaining operators to output
			operator--;
			while (operator != -1 && tokens[token].type != PAREN_LEFT) {
				tokens[token].type = operatorStack[operator].type;
				token++;
				operator--;
			}
			
			operator++; // Counteract the --
			
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

	return token;
}
