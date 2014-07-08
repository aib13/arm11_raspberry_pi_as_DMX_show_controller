#include "assemble.h"

/* hash function gets a string and then returns an unsigned integer
 * that is unique to every instruction that assembler needs to know
 */
unsigned int hash(char* t) {
	return (777 * (t[0] ^ t[1]) ^ 32 * (t[2] ^ t[3])) % 25;
}
