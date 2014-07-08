#include "assemble.h"

void resolveAddress(int* r, char* arg2, char* arg3, char* arg4, char* arg5) {
	//To check if [Rn] or [Rn
	int closingBracketIndex = isInClear(']', arg2, MAX_ARG_SIZE);

	//Set Rn
	int Rn = strToInt(arg2, MAX_ARG_SIZE);
	setBitsAt(r, 16, &Rn, 0, 4);

	//Check if [Rn] or [Rn
	if(closingBracketIndex >= 0) {
		//Pre-index ldr / str Rd, [Rn] case
		if(arg3[0] == '\0') {
			//SET Pre-index
			setBitAt(r, 24, 1);
		} else {
			/* POST-INDEXING
			 * It is of the following two forms
			 * [Rn], #expression
			 * Rn], {+/-}Rm{, <shift>}
			 */
			calcAddress(r, 0, arg3, arg4, arg5);
		}
	} else {
		//PRE-INDEXING
		//SET Pre-index
		setBitAt(r, 24, 1);

		/* It is of the following two forms
		 * Note the bracket close in the second one
		 * [Rn, #expression]
		 * [Rn, {+/-}Rm{, <shift>}]
		*/
		calcAddress(r, 1, arg3, arg4, arg5);
	} //endof if(closingBracketIndex >= 0)
}

int ldr(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6) {
	int r = 0xe4900000;

	//Write Rd
	int Rd = strToInt(arg1, MAX_ARG_SIZE);
	setBitsAt(&r, 12, &Rd, 0, 4);

	//=expression case
	if(arg2[0] == '=') {
		//SET PRE-INDEXING
		setBitAt(&r, 24, 1);

		//SET BASE-REGISTER
		int pc = 0xf;
		setBitsAt(&r, 16, &pc, 0, 4);

		//Get the pointers of the memory and the number instructions from
		//string values
		int** memory = (int**)strtol(arg3, NULL, 0);
		int memory_size = (int)strtol(arg5, NULL, 0);

		//Convert =expression to an integer
		int expression = strToInt(arg2, MAX_ARG_SIZE);

		//Expand the intructions array by 1
		*memory = (int*)realloc(*memory, sizeof(int) * (memory_size + 1));

		//Write the new expression into the memory
		(*memory)[memory_size] = expression;

		int current = atoi(arg4) + 2;
		int offset = (memory_size - current) * 4;

		setBitsAt(&r, 0, &offset, 0, 12);

		//Increment extra data
		int* num_extra_data = (int*)strtol(arg6, NULL, 0);
		(*num_extra_data)++;
	} else {
		//Resolve the general address
		resolveAddress(&r, arg2, arg3, arg4, arg5);
	} //endof if(arg2[0] == '=') else

	return r;
}

int str(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6) {
	int r = 0xe4800000;

	//Write Rd
	int Rd = strToInt(arg1, MAX_ARG_SIZE);
	setBitsAt(&r, 12, &Rd, 0, 4);

	//Resolve the general address
	resolveAddress(&r, arg2, arg3, arg4, arg5);
	return r;
}
