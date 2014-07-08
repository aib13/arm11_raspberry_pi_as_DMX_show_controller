#include "assemble.h"

int (*funcs[25])(char* arg1, char* arg2, char* arg3,
				 char* arg4, char* arg5, char* arg6) = {
	&ble, &teq, &mul, &tst, &bgt, & and , &mla, NULL ,
	&mov, &bge, &orr, NULL, &andeq, &rsb, &str, &ldr,
	&add, &cmp, &beq, &bne, &blt, &b, &eor, &sub,
	&lsl
};


int main(int argc, char **argv) {
	/*
	  checks if the right number of arguments have been provided
	  exits function if the number of arguments doesnt equals to 2
	*/

	if(argc != 3) {
		printf("ERROR: did not provide the correct number of arguments\n");
		exit(EXIT_FAILURE);
	}

	printf("Starting assembler\n");
	//---------------------TEXT_READER---------------------
	//Creates an instance of a map, mapping lables onto values;
	KeyValue labels[MAX_LABELS];

	KeyValue kv;

	memset(kv.key, '\0', MAX_KEY_SIZE);
	memset(kv.value, '\0', MAX_VALUE_SIZE);

	for(int i = 0; i < MAX_LABELS; i++) {
		labels[i] = kv;
	}

	/*Creates a buffer to store the instructions read
	  through the text reader.
	  By default the number of chars per line is less than 512.
	  */
	char buff[MAX_BUFF_SIZE][CHAR_PER_LINE];

	//printf("Going to read the file: %s\n", argv[1]);
	int numRead = readFile(buff, argv[1]);
	printf("Number of valid lines read: %d\n", numRead);

	//printf("Starting labeliser\n");
	//-------------------	LABELISER     ------------------
	/*labelise should return the number of valid instructions,
	  along with updating the map to contain any labels
	  detected during parsing the buffer.
	*/
	int num_Ins = labeliser(buff, labels, numRead);
	printf("Number of instructions: %d\n", num_Ins);
	//-------------  TOKENISER + HASH_FUNCTIONS   ------------

	/* The overall loop runs as follows:
		retrieve a line from the buffer, tokenise the result,
		returning a string array of tokenised instruction,

		this is then passed into the hash function to be parsed into an
		unsigned integer

		finally the integer is passed to the hashed table function
		result is stored in the memory allocated for output values
	*/

	printf("Initialized memory array\n");
	//allocates memory space to store the number of instructions
	int* memory = (int*)calloc(num_Ins, sizeof(int));

	//printf("Creating tokenized instruction array\n");
	//stores a single string array tokenised by the tokeniser function
	char tokens[MAX_ARGS][MAX_ARG_SIZE];

	//stores the int obtained from running the hash function with current_ins
	unsigned int  hashed_ins = 0;
	int num_extra_data = 0;

	int buffindex = 0;

	for(int i = 0; i < num_Ins; ++i) {
		if(buff[buffindex][0] != 0) {
			//printf("Tokenizing instuction: %s", buff[buffindex]);
			//tokeniser returns an address to an string array
			tokeniser(buff[buffindex], labels, tokens);

			//If it is a branching function
			if(tokens[0][0] == 'b') {
				//Pass in its location as second argument
				sprintf(((char*)&tokens[2]), "%d", i);
			}

			//It it is a ldr =expression function
			if(tokens[2][0] == '=') {
				int expression = strToInt(tokens[2], MAX_ARG_SIZE);

				if(expression < 256) {
					strcpy(tokens[0], "mov");
					tokens[2][0] = '#';
				} else {
					sprintf(((char*)&tokens[3]), "%ld", (long)(&memory));
					sprintf(((char*)&tokens[4]), "%d", i);
					sprintf(((char*)&tokens[5]), "%d", num_Ins + num_extra_data);
					sprintf(((char*)&tokens[6]), "%ld", (long)&num_extra_data);
				}
			}

			/*for(int j = 0; j < MAX_ARGS; ++j) {
				printf("tokens[%d]=%s\n", j, tokens[j]);
			}*/

			hashed_ins = hash(tokens[0]);
			//printf("Hash number for %s = %d\n", tokens[0], hashed_ins);

			//Redirect to MOV if LSL
			//Convert lsl Rn, <#expression>
			//To mov Rn, Rn, lsl <#expression>
			if(hashed_ins == 24) {
				hashed_ins = 8;
				strcpy(tokens[4], tokens[2]);
				strcpy(tokens[2], tokens[1]);
				strcpy(tokens[3], "lsl");
			}

			//"Beautiful!" -- Tony Field
			int r = funcs[hashed_ins](
							tokens[1],
							tokens[2],
							tokens[3],
							tokens[4],
							tokens[5],
							tokens[6]
						);
			memory[i] = r;
		} else {
			--i;
		}

		++buffindex;
	}

	//-------------------BINARY WRITER----------------------
	writeBin(argv[2], memory, num_Ins + num_extra_data);
	//frees memory allocated to store instructions;
	free(memory);

	//Notify user
	printf("Done!\n");

	return EXIT_SUCCESS;
}
