#include "assemble.h"

//label of an instruction is (i+1)
int isLabel (char* line, int* index) {
	for (int i = 0; i < CHAR_PER_LINE; ++i) {
		if (line[i] == ':') {
			*index = i;
			//Return true
			return 1;
		}
	}

	//Return false
	return 0;
}

/* Reads though the buff and locates labels,
 * Then populates labels arrays with the labels found with their position
 * in the buff as a WORD ADDRESS
 * Returns: number of valid instructions read (so no labels)
 */
int labeliser(char buff[][CHAR_PER_LINE], KeyValue* labels, int numRead) {
	// Variable to count the number of VALID Instructions
	int ins = 0;
	// Variable to remind the last position of a label
	int positionLabel = 0;

	for(int i = 0; i < numRead; ++i) {
		int index = 0;

		if(isLabel(buff[i], &index)) {
			strncpy((char*)&labels[positionLabel].key, buff[i], index);
			labels[positionLabel].key[index] = '\n';
			sprintf((char*)&labels[positionLabel].value, "%d", ins);
			memset(buff[i], '\0', CHAR_PER_LINE);
			++positionLabel;
		} else {
			++ins;
		}
	}

	return ins;
}
