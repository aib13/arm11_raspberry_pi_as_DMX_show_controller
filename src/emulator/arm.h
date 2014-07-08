#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "../bitlib/bitlib.h"
#define pc registers[15]
#define cpsr registers[16]

typedef struct {

	int memory[MAX_MEMORY_SIZE];
	int registers[NUMBER_OF_REGISTERS];
} ARM;

//Defined enum for the instruction's type
enum INSTRUCTION_TYPE {

	DATA_PROCESSING,
	MULTIPLY,
	SINGLE_DATA_TRANSFER,
	BRANCH

};

//Defined in binread.c
void binRead(int*  mem, char* path);

//Defined in printARM
void printARM (ARM* arm);
void printRegisters(ARM* arm);
void printMemory(ARM* arm);

//Defined in utils.c
int checkCond(int* IR, ARM* arm);
enum INSTRUCTION_TYPE getType(int* instruction);
int reverseInt(int number);
int rotateRight(int n, unsigned int d);
int calcOperand2(int* ir, ARM* arm);
int convertGpioAddress(int* n);
void setGpioAddress(ARM* arm);
void clearGpioAddress(ARM* arm);

//Defined in dataprocessing.c
void dataprocessing(int* ir, ARM* arm);

//Defined in branch.c
void branch(int* ir, ARM* arm);

//Defined in multiply.c
void multiply(int* ir, ARM* arm);

//Define in single_data_transfer.c
void single_data_transfer(int* ir, ARM* arm);
