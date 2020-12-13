#include "header.h"

// Pop top of operator stack into output stack
void popOperatorOutput(struct Token *tokens, int *token, struct Token *operatorStack, int *operator) {
	if (*operator == -1) {
		puts("; ERR Popping negative value");
		return;
	}

	(*token)++;

	// Copy structure value
	tokens[*token] = operatorStack[*operator];

	// Allow values at the bottom to be popped
	if (*operator != 0) {
		(*operator)--;
	}
}

int parse(struct Token *tokens, char string[]) {
	// Token read for 
	size_t c = 0;
	struct Token reading;

	// Operator stack
	struct Token operatorStack[10];
	int operator = -1;
	int token = -1;

	// The shunting yard algorithm.
	// Designed by Edsger Dijkstra in 1961.
	while (!lex(&reading, string, &c)) {
		if (reading.type == INTEGER) {
			token++;
			tokens[token] = reading;
		} else if (reading.type == PAREN_LEFT || reading.type == TEXT) {
			operator++;
			operatorStack[operator] = reading;
		} else if (reading.type == PAREN_RIGHT) {
			while (operator != -1 && operatorStack[operator].type != PAREN_LEFT) {
				if (operator < -1) {
					puts("; ERR: Mismatched parenthesis.1");
					return -1;
				}
				
				popOperatorOutput(tokens, &token, operatorStack, &operator);
			}

			if (operatorStack[operator].type == PAREN_LEFT) {
				// Discard left parenthesis
				operator--;
			} else if (operatorStack[operator].type == TEXT) {
				// Pop functions
				popOperatorOutput(tokens, &token, operatorStack, &operator);
			}

			// Pop the remaining operators to the output
			// while (operator != -1) {
				// popOperatorOutput(tokens, &token, operatorStack, &operator);
			// }
		} else {
			// Else, it is an operator. (+, *, /)
	
			// Push remaining operators to output
			while (operator != -1 && operatorStack[operator].type != PAREN_LEFT) {
				popOperatorOutput(tokens, &token, operatorStack, &operator);
			}
			
			operator++; // Counteract the -1
			
			operatorStack[operator].type = reading.type;
		}
	}

	if (operatorStack[operator].type == PAREN_LEFT
		|| operatorStack[operator].type == PAREN_RIGHT) {
		puts("; ERR: Mismatched parenthesis.3");
		return -1;
	}
	
	// Push the remaining operators to the output
	while (operator != -1) {
		token++;
		tokens[token] = operatorStack[operator];
		operator--;
	}

	return token + 1;
}
