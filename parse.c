#include "header.h"

// Pop top of operator stack into output stack
void popOperatorOutput(struct Token *tokens, size_t *token, struct Token *operatorStack, int *operator) {
	if (*operator == -1) {
		puts("; ERR Popping negative value");
		return;
	}

	// Copy structure value
	tokens[*token] = operatorStack[*operator];
	(*token)++;

	// Allow values at the bottom to be popped
	if (*operator != 0) {
		(*operator)--;
	}
}

int parse(size_t *c, struct Token *tokens, size_t *token, char string[]) {
	*token = 0;
	// Token read for lex
	struct Token reading;

	// Operator stack
	struct Token operatorStack[10];
	int operator = -1;
	
	int blockType;

	// Based on the Shunting Yard Algorithm
	// Designed by Edsger Dijkstra in 1961.
	while (!lex(&reading, string, c)) {
		switch (reading.type) {
		case BRACKET_LEFT:
			return blockType;
		case BRACKET_RIGHT:
			return PARSE_END;
		case FILE_END:
			return PARSE_EOF;
		case SEMICOLON:
			goto endRead;
		case INTEGER:
			tokens[*token] = reading;
			(*token)++;
			break;
		case PAREN_LEFT:
		case EQUAL:
			operator++;
			operatorStack[operator] = reading;
			break;
		case TEXT:
			if (!strcmp(reading.string, "equ")) {
				blockType = PARSE_EQU;
			} else if (!strcmp(reading.string, "nequ")) {
				blockType = PARSE_NEQU;
			} else {
				operator++;
				operatorStack[operator] = reading;
			}
			
			break;
		case PAREN_RIGHT:
			while (operator != -1 && operatorStack[operator].type != PAREN_LEFT) {
				if (operator < -1) {
					puts("; ERR: Mismatched right parenthesis.");
					return PARSE_ERR;
				}
				
				popOperatorOutput(tokens, token, operatorStack, &operator);
			}

			if (operatorStack[operator].type == PAREN_LEFT) {
				// Discard left parenthesis
				operator--;
			} else if (operatorStack[operator].type == TEXT) {
				// Pop functions
				popOperatorOutput(tokens, token, operatorStack, &operator);
			}
			
			break;
		default:
			// Else, it is an operator. (+, *, /)
	
			// Push remaining operators to output
			while (operator != -1 && operatorStack[operator].type != PAREN_LEFT
					&& operatorStack[operator].type != EQUAL) {
				popOperatorOutput(tokens, token, operatorStack, &operator);
			}
			
			operator++; // Counteract the -1
			operatorStack[operator].type = reading.type;
		}
	}

	if (operatorStack[operator].type == PAREN_LEFT
		|| operatorStack[operator].type == PAREN_RIGHT) {
		puts("; ERR: Mismatched left parenthesis.3");
		return PARSE_ERR;
	}
	
	endRead:
	
	// Push the remaining operators to the output
	while (operator != -1) {
		tokens[*token] = operatorStack[operator];
		operator--;
		(*token)++;
	}

	return PARSE_EOS;
}
