#include "header.h"

int main() {
	// Include compiler assembly
	puts("inc \"main.casm\"");
	
	char string[] = "print('0' + 4)";
	struct Token tokens[10];
	int token = parse(tokens, string);
	if (token < 0) {
		return token;
	}

	char *regs[] = {"ga", "gb", "gc", "gd"};
	size_t currentReg = 0;
	
	for (int i = 0; i < token; i++) {
		switch (tokens[i].type) {
		case INTEGER:
			printf("set %s %d\n", regs[currentReg], tokens[i].value);
			currentReg++;
			break;
		case ADD:
			puts("run add");
			currentReg = 1;
			break;
		case TEXT:
			printf("run %s\n", tokens[i].string);
			break;
		}
	}
}
