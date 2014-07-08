#include "arm.h"

/*

  bits 23 to 0 contains a 24 bit signed int,
  named offset, is shifted left 2 bits,
  signed extended to 32 bits and
  added back to the PC

*/

void branch(int* ir, ARM* arm) {
	int offset = getBitsAt(ir, BR_OFFSET_START, BR_OFFSET_LENGTH);
	//Shifted left two bits
	offset <<= 2;
	//Sign extend offset
	int sign_bit = getBitAt(ir, BR_OFFSET_STOP);

	if (sign_bit) {
		offset |= 0xFF000000;
	}

	//Simulating the pipeline behaviour
	arm->pc += 1;
	//Convertion to word address
	arm->pc += (offset / BYTES_IN_WORD);
}
