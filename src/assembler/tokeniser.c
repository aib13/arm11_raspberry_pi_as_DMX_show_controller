#include "assemble.h"

#define FIRST_ARGUMENT_ID   0
#define ARGUMENT_ID         1
#define FIRST_INSTRUCTION   0

// separator between assembly instructions
const char separator[] = " ,";

char* lookUp(char* token, KeyValue* labels) {
	for (int i = 0; i < MAX_LABELS; ++i) {
		if (strcmp(token, labels[i].key) == 0) {
			return labels[i].value;
		}
	}

	printf("ERROR: %s %s\n", "Unknown label", token);
	exit(EXIT_FAILURE);
}

void checkLabel(char tokens[][MAX_ARG_SIZE], KeyValue* labels) {
	// if first instruction starts with a b.
	if (tokens[0][0] == 'b') {
		//replace the label with its value;
		char* value = lookUp(tokens[1], labels);
		memset(tokens[1], '\0', MAX_ARG_SIZE);
		strcpy(tokens[1], value);
	}
}

//Tokenize one single line
void tokeniser(char* line, KeyValue* labels, char tokens[][MAX_ARG_SIZE]) {
	//individual tokens extracted from the string
	char* token;

	//array of strings broken into tokens from the initial string
	for (int i = 0; i < MAX_ARGS; ++i) {
		memset(tokens[i], '\0', MAX_ARG_SIZE);
	}

	/* get the first token */
	token = strtok(line, separator);

	/* store the first token into the tokens array */
	memset(tokens[FIRST_ARGUMENT_ID], '\0', MAX_ARG_SIZE);
	strcpy(tokens[FIRST_ARGUMENT_ID], token);

	/* intialise a variable, say i, that remembers the id of current tokens
	position*/
	int i = FIRST_ARGUMENT_ID + ARGUMENT_ID;

	/* get the rest of the tokens and place them in the tokens array */
	while (token != NULL && i < MAX_ARGS) {
		token = strtok(NULL, separator);

		if(token == NULL) {
			break;
		}

		memset(tokens[i], '\0', MAX_ARG_SIZE);
		strcpy(tokens[i], token);

		++i;
	}

	//assumption that I have a lookUp function
	checkLabel(tokens, labels);
}
