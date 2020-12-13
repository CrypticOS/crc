#include "header.h"

int main(int argc, char *argv[]) {
	if (argc == 1) {return -1;}
	FILE *reader = fopen(argv[1], "r");
	if (reader == NULL) {return -1;}

	char *load = malloc(100 * sizeof(char));
	size_t index = 0;
	int ch;
	while (1) {
		ch = fgetc(reader);
		if (ch == EOF) {
			load[index] = '\0';
			break;
		} else {
			load[index] = ch;
			index++;
		}
	}
	
	fclose(reader);
	
	// Include compiler assembly
	puts("inc \"casm/main.casm\"");
	
	size_t c = 0;
	struct Token tokens[10];
	size_t token = 0;

	while (1) {
		int tryParse = parse(&c, tokens, &token, load);
		if (tryParse == PARSE_EOF) {
			break;
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
				puts("run add");
				currentReg = 1;
				break;
			case TEXT:
				printf("run %s\n", tokens[i].string);
				break;
			case EQUAL:
				i++;
				printf("set %s ga\n", tokens[i].string);
				break;
			}
		}
	}

	free(load);
}
