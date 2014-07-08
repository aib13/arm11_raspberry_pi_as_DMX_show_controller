#include "assemble.h"

/*if the operand is an immediate, modifies the value
  of n and r internally.
*/
void calcOp2Imm(char* arg, int* n, int* r) {
	int op2 = strToInt(arg, MAX_ARG_SIZE);

	/*rotates the immediate value by i every time
	  to check whether can be fit into 8 bits
	  exits if cannot fit, returns an error
	*/
	for(int i = 0; i < 31; i += 2 ) {
		*n = rotateLeft(op2, (unsigned int)i);

		if((unsigned int)*n < 256) {
			*r = i / 2;
			return;
		}
	}

	//if the end of loop is reached without a return, output an error
	printf("ERROR: Cannot compile immediate value for data processing: %d\n", op2);
	exit(EXIT_FAILURE);
}

void resolveOp2(int* r, char* arg2, char* arg3, char* arg4) {
	//Calculating the Operand2
	//if I is set, then operand is an immediate value
	if(arg2[0] == '#') {
		//Set Immediate flag
		setBitAt(r, 25, 1);
		int n = 0, rot = 0;
		//calls function to calculate the immediate operand
		calcOp2Imm(arg2, &n, &rot);
		setBitsAt(r, 0, &n, 0, 8);
		setBitsAt(r, 8, &rot, 0, 4);
	} else {
		//if second operand is a register
		calcAddress(r, 0, arg2, arg3, arg4);
	}
}

/*Generic function to arithmatic operations,
  and, eor, sub, rsb, add, orr

  int r is the Hex decimal Pre-mod version of the cond field and
  the OpCode field specific to the instruction stated.

*/
int arithOp(int r, char* arg1, char* arg2,
			char* arg3, char* arg4, char* arg5, char* arg6) {
	//initialise, setting bits for the Rn and Rd registers
	int Rd = strToInt(arg1, MAX_ARG_SIZE);
	int Rn = strToInt(arg2, MAX_ARG_SIZE);

	//setting the bits for Rn and Rd
	setBitsAt(&r, 16, &Rn, 0, 4);
	setBitsAt(&r, 12, &Rd, 0, 4);

	resolveOp2(&r, arg3, arg4, arg5);

	return r;
}

//Genertic function to test functions tst, cmp and teq
int testOp(int r, char* arg1, char* arg2,
		   char* arg3, char* arg4, char* arg5, char* arg6) {
	//initialising, setting the bits for Rn Register
	int Rn = strToInt(arg1, MAX_ARG_SIZE);
	setBitsAt(&r, 16, &Rn, 0, 4);

	resolveOp2(&r, arg2, arg3, arg4);

	return r;
}


int mov(char* arg1, char* arg2,
		char* arg3, char* arg4, char* arg5, char* arg6) {
	int r = 0xe1a00000;

	//setting the bits for the Rd register
	int Rd = strToInt(arg1, MAX_ARG_SIZE);
	setBitsAt(&r, 12, &Rd, 0, 4);

	resolveOp2(&r, arg2, arg3, arg4);

	return r;
}

//---------------------- ARITHMATIC_OPERATIONS ------------------------

int add(char* arg1, char* arg2,
		char* arg3, char* arg4, char* arg5, char* arg6) {
	int r = 0xe0800000;

	return arithOp(r, arg1, arg2, arg3, arg4, arg5, arg6);
}

int sub(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6) {
	int r = 0xe0400000;

	return arithOp(r, arg1, arg2, arg3, arg4, arg5, arg6);
}
int rsb(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6) {
	int r = 0xe0600000;

	return arithOp(r, arg1, arg2, arg3, arg4, arg5, arg6);
}
int and (char* arg1, char* arg2, char* arg3,
		 char* arg4, char* arg5, char* arg6) {
	int r = 0xe0000000;

	return arithOp(r, arg1, arg2, arg3, arg4, arg5, arg6);
}
int eor(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6) {
	int r = 0xe0200000;

	return arithOp(r, arg1, arg2, arg3, arg4, arg5, arg6);
}
int orr(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6) {
	int r = 0xe1800000;

	return arithOp(r, arg1, arg2, arg3, arg4, arg5, arg6);
}

//---------------------- COMPARE_OPERATIONS ----------------------
int tst(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6) {
	int r = 0xe1100000;

	return testOp(r, arg1, arg2, arg3, arg4, arg5, arg6);
}
int teq(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6) {
	int r = 0xe1300000;

	return testOp(r, arg1, arg2, arg3, arg4, arg5, arg6);
}
int cmp(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6) {
	int r = 0xe1500000;

	return testOp(r, arg1, arg2, arg3, arg4, arg5, arg6);
}
