#include "arm.h"

//Function definitions
int tst(ARM* arm, int Rn, int Rd, int Op2, int S) {
	//Rd = Rn & Op2
	int r = arm->registers[Rn] & Op2;

	//RESULT NOT WRITTEN
	//CFLAG is set by calcOp2 during barrel shifting
	if(S) {
		setBitAt(&(arm->cpsr), ZFLAG, r == 0);
		setBitAt(&(arm->cpsr), NFLAG, getBitAt(&r, INT_SIZE - 1));
	}

	return r; //Return calculated result
}

int teq(ARM* arm, int Rn, int Rd, int Op2, int S) {
	//Rd = Rn ^ Op2
	int r = arm->registers[Rn] ^ Op2;

	//RESULT NOT WRITTEN
	//CFLAG is set by calcOp2 during barrel shifting
	if(S) {
		setBitAt(&(arm->cpsr), ZFLAG, r == 0);
		setBitAt(&(arm->cpsr), NFLAG, getBitAt(&r, INT_SIZE - 1));
	}

	return r; //Return calculated result
}

int and (ARM* arm, int Rn, int Rd, int Op2, int S) {
	//Rd = Rn & Op2
	int r = tst(arm, Rn, Rd, Op2, S);
	arm->registers[Rd] = r;
	return r; //Return calculated result
}
int eor(ARM* arm, int Rn, int Rd, int Op2, int S) {
	//Rd = Rn ^ Op2
	int r = teq(arm, Rn, Rd, Op2, S);
	arm->registers[Rd] = r;
	return r; //Return calculated result
}

int add(ARM* arm, int Rn, int Rd, int Op2, int S) {
	//Rd = Rn + Op2
	int r = arm->registers[Rn] + Op2;
	arm->registers[Rd] = r;

	if(S) {
		setBitAt(&(arm->cpsr), ZFLAG, r == 0);
		setBitAt(&(arm->cpsr), NFLAG, getBitAt(&r, INT_SIZE - 1));
		//CFLAG
		int a = getBitAt(&(arm->registers[Rn]), WORD_SIZE-1);
		int b = getBitAt(&Op2, WORD_SIZE-1);
		int c = getBitAt(&r, WORD_SIZE-1);
		setBitAt(&(arm->cpsr), CFLAG, (c&(!(a))&(!(b))) | ((!(c))&a&b));
	}

	return r; //Return calculated result
}

int cmp(ARM* arm, int Rn, int Rd, int Op2, int S) {
	//Rd = Rn - Op2
	int r = arm->registers[Rn] - Op2;

	//RESULT NOT WRITTEN
	if(S) {
		setBitAt(&(arm->cpsr), ZFLAG, r == 0);
		setBitAt(&(arm->cpsr), NFLAG, getBitAt(&r, INT_SIZE - 1));
		setBitAt(&(arm->cpsr), CFLAG, arm->registers[Rn] >= Op2);
	}

	return r; //Return calculated result
}

int sub(ARM* arm, int Rn, int Rd, int Op2, int S) {
	//Rd = Rn - Op2 == Rn + (-Op2)
	int r = cmp(arm, Rn, Rd, Op2, S);
	arm->registers[Rd] = r;
	return r;
}

int rsb(ARM* arm, int Rn, int Rd, int Op2, int S) {
	//Rd = Op2 - Rn == Op2 + (-Rn)
	//Reverse parameter order
	int r = Op2 - arm->registers[Rn];
	arm->registers[Rd] = r;

	if(S) {
		setBitAt(&(arm->cpsr), ZFLAG, r == 0);
		setBitAt(&(arm->cpsr), NFLAG, getBitAt(&r, INT_SIZE - 1));
		setBitAt(&(arm->cpsr), CFLAG, arm->registers[Rn] <= Op2);
	}

	return r; //Return calculated result
}

int orr(ARM* arm, int Rn, int Rd, int Op2, int S) {
	//Rd = Rn | Op2
	int r = arm->registers[Rn] | Op2;
	arm->registers[Rd] = r;

	//CFLAG is set by calcOp2 during barrel shifting
	if(S) {
		setBitAt(&(arm->cpsr), ZFLAG, r == 0);
		setBitAt(&(arm->cpsr), NFLAG, getBitAt(&r, INT_SIZE - 1));
	}

	return r; //Return calculated result
}

int mov(ARM* arm, int Rn, int Rd, int Op2, int S) {
	//Rd = Operand2 (Rn is ignored
	arm->registers[Rd] = Op2;

	if(S) {
		setBitAt(&(arm->cpsr), ZFLAG, Op2 == 0);
		setBitAt(&(arm->cpsr), NFLAG, getBitAt(&Op2, INT_SIZE - 1));
	}

	return Op2;
}

//Functions array
int (*funcs[16])(ARM* arm, int Rn, int Rd, int Op2, int S) = {
	& and , &eor, &sub, &rsb,
	&add, NULL, NULL, NULL,
	&tst, &teq, &cmp, NULL,
	&orr, &mov, NULL, NULL
};

void dataprocessing(int* ir, ARM* arm) {
	//Fetch things we know
	int Rn = getBitsAt(ir, DP_RN_START, REG_SIZE);
	int Rd = getBitsAt(ir, DP_RD_START, REG_SIZE);
	int S = getBitAt(ir, DP_SET);
	int Op2 = calcOperand2(ir, arm);
	//printf("\n\nOp2: 0x%x\nRn: %d\nRd: %d\n", Op2, Rn, Rd);
	int OpCode = getBitsAt(ir, DP_OPCODE_START, REG_SIZE);
	//printf("OpCode: %d\n\n", OpCode);
	//Execute funtion
	funcs[OpCode](arm, Rn, Rd, Op2, S);
}
