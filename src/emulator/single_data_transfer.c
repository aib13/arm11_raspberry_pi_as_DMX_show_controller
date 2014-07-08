#include "arm.h"

void single_data_transfer(int* ir, ARM* arm) {
	int I  = getBitAt(ir, SDT_IMM);
	int P  = getBitAt(ir, SDT_PFLAG);
	int U  = getBitAt(ir, SDT_UFLAG);
	int L  = getBitAt(ir, SDT_LFLAG);
	int Rn = getBitsAt(ir, SDT_RN_START, REG_SIZE);
	int Rd = getBitsAt(ir, SDT_RD_START, REG_SIZE);
	int offset = 0;

	if(I) {
		int new_ir = *ir;
		//To avoid setting shifter flags set S to 0 with respect to DP
		setBitAt(&new_ir, DP_SET, 0);
		//Flip the Immediate bit as it is the opposite in single-data-transfer
		setBitAt(&new_ir, DP_IMM, getBitAt(ir, DP_IMM) == 0);
		//Offset as a shifted register
		offset = calcOperand2(&new_ir, arm);
	} else {
		//Offset as an unsigned immediate offset
		offset = getBitsAt(ir, SDT_OFFSET_START, 12);
	}

	//printf("offset: %d\n", offset);
	//----------Calculate Byte Address--------
	int byteAddress = arm->registers[Rn];

	if(Rn == 15) {
		//If pc, which is word adress, get the byte address back
		++byteAddress; //Emulate pipeline to get pc = pc + 8
		byteAddress *= 4;
	}

	//---------Check Pre-Post Index----------
	//If p is true then pre-index else post-index
	if(P) {
		//Add or subtract the offset
		byteAddress = U ? byteAddress + offset : byteAddress - offset;
	} else {
		//Update the Rn register but don't change the byteAddress
		arm->registers[Rn] = U ? byteAddress + offset : byteAddress - offset;
	}

	//----------Convert GPIO Address----------
	if(convertGpioAddress(&byteAddress)) return; //If invalid access, abort

	//----------Check Out of bounds-----------
	if(byteAddress >= (1 << 16)) {
		printf("Error: Out of bounds memory access at address 0x%.8x\n", byteAddress);
		return;
	}

	//-------------Calculate word address--------------
	int wordAddress = byteAddress / 4;
	int msWord = (wordAddress == MAX_MEMORY_SIZE - 1) ? 0 : arm->memory[wordAddress + 1];
	long concatWord = concatInts(msWord, arm->memory[wordAddress]);
	int startByte = byteAddress % 4;
	//printf("wordAddress: %d\nStartbyte: %d\n", wordAddress, startByte);
	//printf("ConcatWord: %lx\n", concatWord);

	//----------LOAD | STORE---------
	//If l is true, then load else store
	if(L) {
		//LOAD
		arm->registers[Rd] = (int)lgetBitsAt(&concatWord, startByte * 8, WORD_SIZE);
	} else {
		//STORE
		long lRd = (long)((unsigned long)arm->registers[Rd]);
		//Write into the concatenated word
		lsetBitsAt(&concatWord, startByte * 8, &lRd, 0, WORD_SIZE);
		//Write the new concatenated word back into the memory
		arm->memory[wordAddress] = (int)concatWord;

		if(wordAddress != MAX_MEMORY_SIZE - 1) {
			arm->memory[wordAddress + 1] = (int)(concatWord >> 32);
		}
	}
}
