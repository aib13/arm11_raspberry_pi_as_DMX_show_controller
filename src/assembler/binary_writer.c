#include "assemble.h"

/*
 The function writes the content
 into a binary file
*/
void writeBin(char* path, int* instructions, int num_instructions) {
	// Creating the output file
	FILE *output = fopen(path, "wb");

	// Verifying the output
	if(output == NULL) {
		printf("ERROR: Cannot open file: %s\n", path);
		exit(EXIT_FAILURE);
	}

	for( int i = 0; i < num_instructions; ++i) {
		fwrite( &(instructions[i]) , sizeof(int) , 1, output);
	}

	fclose(output);
}
