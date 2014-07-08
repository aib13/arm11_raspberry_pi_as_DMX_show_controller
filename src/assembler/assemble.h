#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bitlib/bitlib.h"

#define MAX_KEY_SIZE 		20
#define MAX_VALUE_SIZE 		10

#define CHAR_PER_LINE  	 	512
#define MAX_ARGS  	 		7
#define MAX_ARG_SIZE 		(CHAR_PER_LINE / MAX_ARGS)
#define MAX_LABELS		 	50
#define MAX_BUFF_SIZE	 	1000
#define MAX_LABELS			50

typedef struct {
	char key[MAX_KEY_SIZE];
	char value[MAX_VALUE_SIZE];
} KeyValue;

//Defined in text_reader.c
int readFile(char buff[][CHAR_PER_LINE], char* path);

//Defined in binary_writer.c
void writeBin(char* path, int* instructions, int num_instructions);

//Defined in tokeniser.c
void tokeniser(char* line, KeyValue* labels, char tokens[][MAX_ARG_SIZE]);

//Defined in labeliser.c
int labeliser(char buff[][CHAR_PER_LINE], KeyValue* labels, int numRead);

//Defined in hash.c
unsigned int hash(char* t);

//Defined in utils.c
int rotateLeft(int value, unsigned int shift);
int strToInt(char* str, int str_size);
int isInClear(char c, char* str, int str_size);
int getShiftCode(char* str);
void calcAddress(int* r, int isPreIndex, char* arg3, char* arg4, char* arg5);

//Defined in dataProcessing.c
int add(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int mov(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int sub(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int rsb(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int and (char* arg1, char* arg2, char* arg3,
		 char* arg4, char* arg5, char* arg6);
int eor(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int orr(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int tst(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int teq(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int cmp(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);

//Defined in multiply.c
int mul(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int mla(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);

//Defined single_data_transfer.c
int ldr(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int str(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);

//Defined in branch.c
int beq(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int bne(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int bge(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int blt(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int bgt(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int ble(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int b(char* arg1, char* arg2, char* arg3,
	  char* arg4, char* arg5, char* arg6);

//Defined in special.c
int lsl(char* arg1, char* arg2, char* arg3,
		char* arg4, char* arg5, char* arg6);
int andeq(char* arg1, char* arg2, char* arg3,
		  char* arg4, char* arg5, char* arg6);
