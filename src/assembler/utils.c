#include "assemble.h"

int rotateLeft(int n, unsigned int d) {
	return (int)(((unsigned int)n) >> (32 - d)) | (n << d);
}

int strToInt(char* str, int str_size) {
	//Seek until the first valid number
	//And then convert it from that offset
	//Note that the number could be negative as well
	for(int i = 0; i < str_size; ++i) {
		if((str[i] >= '0' && str[i] <= '9') || str[i] == '-') {
			return (int)strtol(str + i, NULL, 0);
		}
	}

	return 0;
}

int isInClear(char c, char* str, int str_size) {
	for(int i = 0; i < str_size; ++i) {
		if(str[i] == c) {
			str[i] = '\0';
			return i;
		} else if(str[i] == '\0') {
			//If it is the end of the string
			break;
		}
	}

	return -1;
}

int getShiftCode(char* str) {
	//It can be lsl, lsr, asr, ror
	if(str[0] == 'l') {
		if(str[2] == 'l') {
			//lsl
			return 0;
		} else {
			//lsr
			return 1;
		}
	} else if (str[0] == 'a') {
		return 2;
	} else if (str[0] == 'r') {
		return 3;
	}

	printf("ERROR: Unknown shift command: %s\n", str);
	exit(EXIT_FAILURE);
}

void calcAddress(int* r, int isPreIndex, char* arg3, char* arg4, char* arg5) {
	/* POST-INDEXING
	 * It is of the following two forms
	 * [Rn], #expression
	 * [Rn], {+/-}Rm{, <shift>}
	 *
	 * PRE-INDEXING
	 * It is of the following two forms
	 * Note the bracket close in the second one
	 * [Rn, #expression]
	 * [Rn, {+/-}Rm{, <shift>}]
	*/
	if(arg3[0] == '#') {
		//#expression] form (']' depends on indexing)
		if(isPreIndex) isInClear(']', arg3, MAX_ARG_SIZE);

		int offset = strToInt(arg3, MAX_ARG_SIZE);

		//offset must fit in 12bits as signed number
		if(offset > 0x7ff || offset < ((int)(0xfffff800))) {
			printf("ERROR: Cannot fit offset value \"%d\" to 12 bits in ldr\n", offset);
			exit(EXIT_FAILURE);
		}

		//If it does continue
		int isNeg = getBitAt(&offset, 31);

		//Set U Flag, add or subtract offset
		setBitAt(r, 23, isNeg == 0);

		if(isNeg) {
			offset = -(offset);
		}

		//Set the offset
		setBitsAt(r, 0, &offset, 0, 11);

	} else {
		//{+/-}Rm{, <shift>}] ']' depends on indexing form
		//SET IFLAG as offset is a shifted register
		//NOTE OPPOSITE CASE IN SINGLE DATA TRANSFER
		if(getBitAt(r, 26)) setBitAt(r, 25, 1);

		//Is the bracket closing? If yes, then there is no shift
		//Rm] or Rm, <shift>] for pre-index
		//Rm, <shift>

		//Clear any closing brackets from Rm] in pre-indexing
		if(isPreIndex) isInClear(']', arg3, MAX_ARG_SIZE);

		//Check positive or negative and get Rm value
		int Rm;

		if(arg3[0] == '-') {
			//Set U Flag to subtract offset
			setBitAt(r, 23, 0); //subtract
			//Omit '-' when parsing
			Rm = strToInt(arg3 + 1, MAX_ARG_SIZE - 1);
		} else {
			//By default the UFLAG is set to add
			Rm = strToInt(arg3, MAX_ARG_SIZE);
		}

		//Set Rm
		setBitsAt(r, 0, &Rm, 0, 4);

		if(arg4[0] != '\0') {
			/*THERE IS SHIFTING
			* It can be 1 of the following
			* <shift> <register>]
			* <shift> <#expression>]
			* ']' depend on indexing type
			*/

			//Get and set the shift code
			int shiftCode = getShiftCode(arg4);
			setBitsAt(r, 5, &shiftCode, 0, 2);

			//get rid of closing brackets if pre-indexing
			if(isPreIndex) isInClear(']', arg5, MAX_ARG_SIZE);

			//Check the second argument to shift
			if(arg5[0] == '#') {
				//<#expression> form
				int shift_amount = strToInt(arg5, MAX_ARG_SIZE);

				//Shift amount must fit into 5 bits
				if(shift_amount > 0x1f) {
					printf("ERROR: Too large immediate shift value: %d\n", shift_amount);
					exit(EXIT_FAILURE);
				}

				//Set the shift amount
				setBitsAt(r, 7, &shift_amount, 0, 5);
			} else {
				//<register> form
				//Set register shift flag
				setBitAt(r, 4, 1);

				//Get and set Rs
				int Rs = strToInt(arg5, MAX_ARG_SIZE);
				setBitsAt(r, 8, &Rs, 0, 4);
			}
		}
	} //endof if(arg3[0] == '#') else
}
