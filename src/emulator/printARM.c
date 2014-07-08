#include "arm.h"

/*
*
* printRegisters takes the input a pointer to the ARM struct then it prints the
* state of all registers taking into account the display from the test cases,
* that is it prints their content in both integers and hexadecimal format.
*
*/

void printRegisters(ARM* arm) {
	printf("Registers:\n");

	// Format of print is "$8  : ", notice the two spaces after 8
	for (int i = FIRST_REG; i < DOUBLE_DIG_REG; ++i) {
		printf("$%d  : %10d (0x%.8x)\n",
			   i, arm->registers[i], arm->registers[i]);
	}

	// Format of print is "$10 : ", notice that it only includes one space
	// after the register number, and not two as above.
	for (int i = DOUBLE_DIG_REG; i <= LAST_REG; ++i) {
		printf("$%d : %10d (0x%.8x)\n",
			   i, arm->registers[i], arm->registers[i]);
	}

	printf("PC  : %10d (0x%.8x)\n", (arm->pc + 1) * 4, (arm->pc + 1) * 4);
	printf("CPSR: %10d (0x%.8x)\n", arm->cpsr, arm->cpsr);
}

/*
*
* printMemory takes a pointer to the ARM struct and goes from MEM_ADDR, that is
* the start location of memory as defined in "arm.h", i.e 0, and loops to the
* last memory location, i.e MAX_MEMORY_SIZE.
*
*
* printMemory follows the test cases printing, that is hexadecimal values, and
* prints the "non-zero location" memory.
*
*/

void printMemory(ARM* arm) {
	printf("%s\n", "Non-zero memory:");

	for (int i = MEM_ADDR; i < MAX_MEMORY_SIZE; ++i) {
		if (arm->memory[i]) {
			// 1: First parameter to print is 4 * i because our memory is
			//    word-addressable, and their is byte-addressable
			// 2: Second parameter is the reverse number we store in memory
			//    because when we read, fread reverses the order of bytes read.
			printf("0x%.8x: 0x%.8x\n", 4 * i , reverseInt(arm->memory[i]));
		}
	}
}

/*
*
* printARM takes a pointer to the ARM struct, and calls both printRegisters and
* printMemory on the ARM struct pointer.
*
* this is the method we use to print the emulator status.
*
*/

void printARM (ARM* arm) {
	printRegisters(arm);
	printMemory(arm);
}
