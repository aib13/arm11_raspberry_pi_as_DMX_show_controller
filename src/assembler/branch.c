#include "assemble.h"

int r = 0;
int offset;
int cond = 0x0;

int calculate_result(int cond) {

	return (cond << 28) | (0x0A000000) | offset;

}

int solve_branching(char* arg1, char* arg2, int cond) {

	int target = atoi(arg1);
	int current = atoi(arg2);
	current += 2;
	offset = (((target - current) << 2) >> 2) & 0x00FFFFFF;

	return calculate_result(cond);

}

int beq(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6) {
	return solve_branching(arg1, arg2, 0x0);
}

int bne(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6) {
	return solve_branching(arg1, arg2, 0x1);
}

int bge(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6) {
	return solve_branching(arg1, arg2, 0xA);
}

int blt(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6) {
	return solve_branching(arg1, arg2, 0xB);
}

int bgt(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6) {
	return solve_branching(arg1, arg2, 0xC);
}

int ble(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6) {
	return solve_branching(arg1, arg2, 0xD);
}

int b(char* arg1, char* arg2, char* arg3,
	  char* arg4, char* arg5, char* arg6) {
	return solve_branching(arg1, arg2, 0xE);
}
