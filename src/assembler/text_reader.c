#include "assemble.h"

/*

 The function reads the content from a text file
 (represented by the pointer *path)
 and puts it into the buffer,
 passed as a parameter

*/

int readFile(char buff[][CHAR_PER_LINE], char* path) {

	// Creating the input file
	FILE *input = fopen(path, "r");

	// Verifying the input
	if(input == NULL) {
		printf("ERROR: Cannot open file: %s\n", path);
		exit(EXIT_FAILURE);
	}

	// Reading the content of the input file
	int i = 0;

	// Initialise the buffer
	memset(&buff[i], '\0', CHAR_PER_LINE);

	while( fgets(buff[i], CHAR_PER_LINE, input) ) {
		//Only read valid lines that start with a letter
		if((buff[i][0] >= 'a' && buff[i][0] <= 'z') ||
				(buff[i][0] >= 'A' && buff[i][0] <= 'Z')) {
			++i;
		} else {
			if(buff[i][0] != '\n') {
				//Notify user that we are omitting a line
				printf("Omitting line: %s", buff[i]);
			}
		}

		memset(&buff[i], '\0', CHAR_PER_LINE);
	}

	fclose(input);
	return i;
}
