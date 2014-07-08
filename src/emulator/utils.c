#include "arm.h"
#include <assert.h>

/*

    input    - the pointer to the instruction register(int),
	compares - cond values against the CPSR flags
	and returns 0 if codes match,
	otherwise returns 1 for failure;

*/

int checkCond(int* IR, ARM* arm) {
	int cond = getBitsAt(IR, INS_CON_START, INS_CON_SIZE);
	//N_Z_C_V
	int flags = getBitsAt(&(arm->cpsr), INS_CON_START, INS_CON_SIZE);

	switch(cond) {
		// Z set
		case 0:
			return getBitAt(&(arm->cpsr), ZFLAG);

		// Z clear
		case 1:
			return (1 - getBitAt(&flags, 2));

		//N equals V
		case 10:
			if(getBitAt(&flags, 3) == getBitAt(&flags, 0)) {
				return 1;
			}

		// N not equal to V
		case 11:
			if(getBitAt(&flags, 3) != getBitAt(&flags, 0)) {
				return 1;
			} else {
				return 0;
			}

		// Z clear AND (N equals V)
		case 12:
			if((getBitAt(&flags, 3) == getBitAt(&flags, 0)) &&
					getBitAt(&flags, 2) == 0) {
				return 1;
			} else {
				return 0;
			}

		// Z set OR (N not equals to V)
		case 13:
			if((getBitAt(&flags, 3) != getBitAt(&flags, 0)) ||
					getBitAt(&flags, 2) == 1) {
				return 1;
			} else {
				return 0;
			}

		// AL_Flag
		case 14:
			return 1;

		default:
			return 0;
	}
}

/*

    the function obtaines the type from an instruction passed throughout
    a pointer, by checking the bits 27 and 26. From here you could only find out
    whether the instruction is Single Data Transfer, i.e bit 27 is 0 and
     bit 26 is 1, or the instruction is Branch, i.e bit 27 is 1 and
     bit 26 is 0.

    In the case when both above bits are 0 there are a few cases to be checked:

      i) if bit 25 is 1 then the instructions has to be a Data Processing
          because Multiply *ALWAYS* has bit 25 set to 0.

      ii) bit 25 is 0, then this gets slightly more complex, but before
          explaining the cases this case gets dived into, at this point
          we know for sure that Operand2 (in the case where the
          instruction would be a Data Processing) is a shifted register,
          that is the bits from 3 to 0 represent a register, and the bits
          form 11 to 4 the shift. So here are the possible cases when
          bit 25 is 0:

                    a) if bit 4 is 0, then it's a Data Processing Instruction, because Multiply has bit 4 *ALWAYS* 1.

                    b) otherwise, if bit 4 is 1 as well, then the difference comes
                       from the bit 7. If this is a Multiply instruction then
                       bit 7 is 1, but if it's a Data Processing Instruction,
                       then bit 7 is forced (taking into account that bit 4 is
                       1) to be 0. So now whether you're dealing with one
                       instruction or the other one simply results from the
                       bit 7.

*/

enum INSTRUCTION_TYPE getType( int* instruction) {
	int type = getBitsAt(instruction, INS_TYPE_START, INS_TYPE_SIZE);

	switch (type) {
		// Data Processing OR Multiply
		case 0: {
			if( getBitAt(instruction, DP_IMM)) {
				return DATA_PROCESSING;
			}

			if(getBitAt(instruction, 4)) {
				if( getBitAt(instruction, 7) ) {
					return MULTIPLY;
				}

				return DATA_PROCESSING;
			} // case 0

			return DATA_PROCESSING;
		}

		case 1: {
			return SINGLE_DATA_TRANSFER;
		} // case 1

		default: {
			return BRANCH;
		} // default
	}
}

/*
*
*  reverseInt takes a 32-bit number, and it reverses its bytes, i.e first 8-bits
*  become last 8-bits and so on, e.g e5 9f 20 02 becomes 02 20 9f e5.
*
*  Takes byte at position i, starting from FIRST_BYTE_IN_WORD, i.e 0, and shifts
*  it to the left so that it goes to position 3 - i, that to the reversed
*  position, where 3 is the number of the last byte i a word.
*
*  Remember that shifting is a bit operation, that is when we find out by how
*  many bytes we want to shift, we times that number by 8, i.e the number of
*  bits in a byte - or, according to our definitions, BYTE_SIZE constant.
*
*  E.g, suppose (3, 2, 1, 0) are the bytes of a 32-bit number, then say i is 1
*  - that is the byte we want to change, then its position in the new number
*  should be 3 - 1 = 2. So we want to shift our byte by 2 * 8 = 16 BITS.
*
*/

int reverseInt(int number) {
	// r it will be the reversed number. By default, is initialiased by 0.
	int r = 0;

	for(int i = FIRST_BYTE_IN_WORD; i < BYTES_IN_WORD; ++i) {
		r |= getBitsAt(&number, i * BYTE_SIZE, BYTE_SIZE)
			 << (LAST_BYTE_IN_WORD - i) * BYTE_SIZE;
	}

	return r;
}

int rotateRight(int n, unsigned int d) {
	return (int)(((unsigned int)n) >> d) | (n << (INT_SIZE - d));
}

int calcOperand2(int* ir, ARM* arm) {
	//Check for immediate value
	if(getBitAt(ir, DP_IMM)) {
		//If immediate
		int Imm_val = getBitsAt(ir, 0, 8);
		//rotate twice the amount, check spec
		int rot_offset = getBitsAt(ir, 8, 4) * 2;
		return rotateRight(Imm_val, (unsigned int)rot_offset);
	} else {
		//We have a register as Op2 to be shifted
		int Rm = getBitsAt(ir, 0, REG_SIZE);
		int Rm_val = arm->registers[Rm];
		int shift_type = getBitsAt(ir, 5, 2);
		unsigned int shiftAmount = 0; //set below

		//If set, SHIFT BY A REGISTER
		//If not, SHIFT BY A CONSTANT
		if(getBitAt(ir, 4)) {
			int Rs = getBitsAt(ir, 8, REG_SIZE);
			//Bottom byte of register Rs becomes the amount
			shiftAmount = (unsigned int)(arm->registers[Rs] & 0xFF);
		} else {
			shiftAmount = (unsigned int)getBitsAt(ir, 7, 5);
		}

		int r = 0; //return value
		int S = getBitAt(ir, DP_SET);

		//----------BARREL SHIFTER----------
		//For carry flags, see spec page 8 figure 2
		switch(shift_type) {
			//Logical left
			case 0:
				r = Rm_val << shiftAmount;

				if(S) {
					int flag = shiftAmount == 0 ? 0 : getBitAt(&Rm_val, INT_SIZE - shiftAmount);
					setBitAt(&(arm->cpsr), CFLAG, flag);
				}

				return r;

			//Logical right
			//>> on unsigned performs logical right shift
			case 1:
				r = (int)(((unsigned int)Rm_val) >> shiftAmount);
				break;

			//Arithmetic right
			//>> on signed performs arithmetic right shift
			//source: http://en.wikipedia.org/wiki/Bitwise_operations_in_C#Right_shift_.3E.3E
			case 2:
				r = Rm_val >> shiftAmount;
				break;

			//Rotate right:
			case 3:
				r = rotateRight(Rm_val, shiftAmount);
				break;
		}

		//Set common flag for Logical right, arithmetic right and rotate right
		if(S) {
			int flag = shiftAmount == 0 ? 0 : getBitAt(&Rm_val, shiftAmount - 1);
			setBitAt(&(arm->cpsr), CFLAG, flag);
		}

		return r;
	}
}

/* Checks and converts the 3 GPIO addresses:
 * 0x2020 0008 = MAX_MEMORY_SIZE -1
 * 0x2020 0004 = MAX_MEMORY_SIZE -2
 * 0x2020 0000 = MAX_MEMORY_SIZE -3
 * to last memory positions
 * Otherwise it doesn't change the address
 * RETURN: returns 0 if memory location exitst, it doesnt for set and clear
 * Otherwise returns 1 to abort memory access
 */
int convertGpioAddress(int* n) {
	//Look at spec page 19 for addresses
	switch(*n) {
		//GPIO 20-29
		case GPIO_20_29:
			printf("One GPIO pin from 20 to 29 has been accessed\n");
			break;

		//GPIO 10-19
		case GPIO_10_19:
			printf("One GPIO pin from 10 to 19 has been accessed\n");
			break;

		//GPIO 0-9
		case GPIO_0_9:
			printf("One GPIO pin from 0 to 9 has been accessed\n");
			break;

		//CLEAR PIN ADDRESS
		case GPIO_CLEAR:
			printf("PIN OFF\n");
			return 1;

		//SET PIN ADDRESS
		case GPIO_SET:
			printf("PIN ON\n");
			return 1;

		default:
			return 0;
	}

	*n = ((MAX_MEMORY_SIZE - ((*n - GPIO_0_9) / 4)) - 1) * 4;
	return 0;
}

/* Sets the initial values of these GPIO pins
 * which are the same as the addresses
 * check spec page 19, last sentence
 */
void setGpioAddress(ARM* arm) {
	arm->memory[MAX_MEMORY_SIZE - 1] = GPIO_20_29;
	arm->memory[MAX_MEMORY_SIZE - 2] = GPIO_10_19;
	arm->memory[MAX_MEMORY_SIZE - 3] = GPIO_0_9;
}

/* Clears the GPIO addresses as they normally
 * do not exists in the memory, but you realize this
 * when you reach part 3
 */
void clearGpioAddress(ARM* arm) {
	arm->memory[MAX_MEMORY_SIZE - 1] = 0;
	arm->memory[MAX_MEMORY_SIZE - 2] = 0;
	arm->memory[MAX_MEMORY_SIZE - 3] = 0;
}
