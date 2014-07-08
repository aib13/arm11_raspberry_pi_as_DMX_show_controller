#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	
	if(argc != 3) {
		printf("ERROR: did not provide the correct number of arguments\n");
		exit(EXIT_FAILURE);
	}
	
	// Creating the input and output file
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "wb");

	// Verifying the input
	if(input == NULL) {
		return 1;
	}
    
    if(output == NULL) {
		printf("ERROR: Cannot open file: %s\n", argv[3]);
		exit(EXIT_FAILURE);
	}

	char channels[65];
    int number_of_scenes, delay;
    int r = 0;
    
    fscanf(input, "%d", &number_of_scenes);
    fscanf(input, "%d", &delay);
    
    r|=number_of_scenes;
    r<<=16;
    r|=delay;
    
    fwrite( &r, sizeof(int), 1, output);

	for(int i = 1; i <= number_of_scenes; ++i) {

		for(int j = 0; j < 65; j++) {
			channels[j] = 0;
		}
        
        int channel, value;
        
        fscanf(input, "%d", &channel);

		while( channel != 513 ) {
			
            fscanf(input, "%d", &value);

			channels[channel] = (char)value;
            
            fscanf(input, "%d", &channel);

		}

		for(int j = 1; j <= 64; ++j) {
			fwrite( &channels[j], sizeof(char), 1, output);
		}

	}

    fclose(input);
    fclose(output);
    
    return 0;
}
