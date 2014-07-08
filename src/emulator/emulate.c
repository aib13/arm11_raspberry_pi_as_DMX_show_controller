#include <stdbool.h>
#include <string.h>
#include "arm.h"


int main(int argc, char *argv[]) {
	//Check for binary file argument
	if(argc != 2) {
		printf("No binary file provided!\n");
		printf("Usage: emulate [binary_file]\n\n");
		exit(EXIT_FAILURE);
	}

	//----------INITIALIZE-------------
	//Initialize arm struct
	ARM arm;

	//memset(arm.memory, 0, MAX_MEMORY_SIZE); //zero array
	for(int i = 0; i < MAX_MEMORY_SIZE; ++i) {
		arm.memory[i] = 0;
	}

	//GPIO
	setGpioAddress(&arm); //setup GPIO address
	arm.pc = 0; //Execute from the beginning

	//memset(arm.registers, 0, NUMBER_OF_REGISTERS);
	for(int i = 0; i < NUMBER_OF_REGISTERS; ++i) {
		arm.registers[i] = 0;
	}

	arm.cpsr = 0;
	//Read the binary file (program) to the memory, starting from pos0
	binRead(arm.memory, argv[1]);
	int ir = 0; //INSTRUCTION REGISTER

	//-------FETCH-EXECUTE CYCLE-------
	do {
		//FETCH
		ir = arm.memory[arm.pc]; //Fetch the current instruction
		++arm.pc; //Increment pc counter
		//DECODE
		int cond = checkCond(&ir, &arm);

		if(cond) {
			//Determine type
			switch(getType(&ir)) {
				case DATA_PROCESSING:
					dataprocessing(&ir, &arm);
					break;

				case BRANCH:
					branch(&ir, &arm);
					break;

				case MULTIPLY:
					multiply(&ir, &arm);
					break;

				case SINGLE_DATA_TRANSFER:
					single_data_transfer(&ir, &arm);
					break;
			}
		}
	} while (ir != 0);

	//---------CLEAR GPIO VIRTUAL MEMORY-------
	clearGpioAddress(&arm);
	//---------PRINT ARM STATUS-------
	printARM(&arm);
	return EXIT_SUCCESS;
}
