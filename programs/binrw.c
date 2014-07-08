#include<stdio.h>
#include<string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	
	if(argc != 4) {
		printf("ERROR: did not provide the correct number of arguments\n");
		exit(EXIT_FAILURE);
	}
	
	// Creating the input file
	FILE* input1 = fopen(argv[1], "rb");
	FILE* output = fopen(argv[3], "wb");

	// Verifying the input
	if(input1 == NULL) {
		return 1;
	}
    
    if(output == NULL) {
		printf("ERROR: Cannot open file: %s\n", argv[3]);
		exit(EXIT_FAILURE);
	}

	// Reading the content of the input file
	int res = 0;
    int mem;
    
	do {
		res = fread(&mem, sizeof(int), 1, input1);
        
        if(res != 0) {
        
            fwrite(&mem , sizeof(int) , 1, output);
        }
		
	} while(res != 0);
    
    fclose(input1);
    
    FILE* input2 = fopen(argv[2], "rb");
    
    if(input2 == NULL) {
		return 1;
	}
    
    do {
		res = fread(&mem, sizeof(int), 1, input2);
        
        if(res != 0) {
            fwrite(&mem , sizeof(int) , 1, output);
		}
        
	} while(res != 0);

    fclose(input2);
    
    fclose(output);
    
    return 0;
}
