#include<stdio.h>
#include<string.h>

/*

 The function reads the content from a binary file
 (represented by the pointer *path)
 and puts it into the memory, represented by *mem,
 passed as a parameter

*/

void binRead(int*  mem, char* path) {
	// Creating the input file
	FILE* input = fopen(path, "rb");

	// Verifying the input
	if(input == NULL) {
		return;
	}

	// Reading the content of the input file
	int i = 0, res = 0;

	do {
		res = fread(&mem[i], sizeof(int), 1, input);
		++i;
	} while(res != 0);

	fclose(input);
}
