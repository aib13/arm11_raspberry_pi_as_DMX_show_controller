#include "assemble.h"

int mul (char* arg1, char* arg2, char* arg3,
		 char* arg4, char* arg5, char* arg6) {

	int r = 0xe0000090;

	int Rd = strToInt(arg1, MAX_ARG_SIZE);
	int Rm = strToInt(arg2, MAX_ARG_SIZE);
	int Rs = strToInt(arg3, MAX_ARG_SIZE);

	setBitsAt(&r, 16, &Rd, 0, 4);
	setBitsAt(&r, 0, &Rm, 0, 4);
	setBitsAt(&r, 8, &Rs, 0, 4);

	return r;
}

int mla (char* arg1, char* arg2, char* arg3,
		 char* arg4, char* arg5, char* arg6) {

	int r = 0xe0200090;

	int Rd = strToInt(arg1, MAX_ARG_SIZE);
	int Rm = strToInt(arg2, MAX_ARG_SIZE);
	int Rs = strToInt(arg3, MAX_ARG_SIZE);
	int Rn = strToInt(arg4, MAX_ARG_SIZE);

	setBitsAt(&r, 16, &Rd, 0, 4);
	setBitsAt(&r, 0, &Rm, 0, 4);
	setBitsAt(&r, 8, &Rs, 0, 4);
	setBitsAt(&r, 12, &Rn, 0, 4);

	return r;
}
