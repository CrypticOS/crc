#include "header.h"

int main() {
	puts("inc \"$kernel/main.casm\"");
	puts("jmp top\ninc \"$kernel/math/add.casm\"\ntop:");
	
	char string[] = "1 + 1 + 2";
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
			puts("run k.add");
			currentReg = 0;
			printf("set %s gr\n", regs[currentReg]);
			currentReg++;
			break;
		}
	}

	puts("add ga '0'\nprt ga");
}
