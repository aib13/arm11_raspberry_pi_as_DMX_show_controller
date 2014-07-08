#include "arm.h"

/*
*
* mul is the execute function of multiply instruction. Takes a pointer to the
* ARM struct, the Accumulate bit, the Set bit, the destionation register Rd, the
* operand registers Rn, Rs and Rm.
*
* It does calculate the result based on the Accumulate bit, and places the
* result in the Rd, i.e the destination register.
*
* If the Set bit is 1, then it updates the CPRS flags according to the
* specification:
*   - NFLAG is set to the 31st bit of the result
*	- ZFLAG is set iff the result is 0.
*/

void mul(ARM* arm, int Acc, int Set, int Rd, int Rn, int Rs, int Rm) {
	switch(Acc) {
		// Acc = 0 -> no accumulate mode
		case 0: {
			// Rd = Rm * Rs
			arm->registers[Rd] = arm->registers[Rm] * arm->registers[Rs];
			break;
		}

		// Acc = 1 -> accumulate mode
		case 1: {
			// Rd = Rm * Rs + Rn
			arm->registers[Rd] =
				arm->registers[Rm] * arm->registers[Rs] + arm->registers[Rn];
			break;
		}
	}

	if (Set) {
		// Set the NFLAG of CPSR to be 31st bit of the result, i.e Rd
		setBitAt(&(arm->cpsr), NFLAG,
				 getBitAt(&(arm->registers[Rd]), INT_SIZE - 1));
		// Set the ZFLAG of CPSR iff the result is 0.
		setBitAt(&(arm->cpsr), ZFLAG, arm->registers[Rd] == 0);
	}
}



/*
*
* multiply is the core function of the multiply instruction. It takes an
* instruction pointer and an ARM struct pointer and it does the following
*
* 1) it decodes the necessary information from the instruction, e.g Acc bit etc.
* 2) it calls the execute function mul to perform the multiply instruction
*
*/

void multiply(int* ir, ARM* arm) {
	/*
	*	We start by fetching things we know. And they are classified as follows:
	*
	*   <<IMPORTANT BITS>>
	*
	*	Acc - the acummulate bit (21st bit of ir)
	*   Set - the set bit (20th bit of ir)
	*
	*   <<Registers>>
	*
	*   Rd  - destination register (starts at MP_RD_START, i.e 16)
	*	Rn  - operand register (starts at MP_RN_START, i.e 12)
	* 	Rs  - operand register (starts at MP_RS_START, i.e 8)
	*   Rm  - operand register (starts at MP_RM_START, i.e 0)
	*/
	int Acc = getBitAt(ir, ACC_START);
	int Set = getBitAt(ir, SET_START);
	int Rd = getBitsAt(ir, MP_RD_START, REG_SIZE);
	int Rn = getBitsAt(ir, MP_RN_START, REG_SIZE);
	int Rs = getBitsAt(ir, MP_RS_START, REG_SIZE);
	int Rm = getBitsAt(ir, MP_RM_START, REG_SIZE);
	//Calling the mul function, i.e the execute function of multiply instruction
	mul(arm, Acc, Set, Rd, Rn, Rs, Rm);
}
