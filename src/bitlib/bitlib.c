#include <assert.h>
#include <stdio.h>
#include "bitlib.h"

#define WORD_SIZE sizeof(int) * 8

/*takes in a pointer to a binary number,
  and outputs the bitwise number at index,
  bit 31 is on the left_hand side
*/
int getBitAt(int* n, int index) {
	assert((index < WORD_SIZE) && (index >= 0));
	int maskbit = 1 << index;
	return (int)(((unsigned int)(*n & maskbit)) >> index);
}

int lgetBitAt(long* n, int index) {
	assert((index < WORD_SIZE*2) && (index >=0));
	//1 by default is considered an integer so we must declare a long first
	long mask = 1;
	mask <<= index;
	return (int)(((unsigned long)(*n & mask)) >> index);
}


/* takes in a pointer to a binary number,
   and set the bit at index to 1 or 0
*/
void setBitAt(int* n, int index, int new_digit) {
	assert((new_digit == 1 || new_digit == 0)
		   && (index < WORD_SIZE) && (index >= 0));
	/*The idea is you first clear the bit to 0
	 * and then write the new one, ~ operator
	 * flips bits so ~(1100) = 0011
	 */
	*n = (*n & ~(1 << index)) | (new_digit << index);
}

/* takes in a pointer to a binary number
   and return the value of consecutive bits
   between start and the number of bits required
*/
int getBitsAt(int* n, int start, int size) {
	assert(size >= 0 && size <= WORD_SIZE);
	int mask = 0;

	for(int i = 0; i < size ;  i++) {
		mask |= (1 << (start + i));
	}

	return (int)(((unsigned int)(*n & mask)) >> start);
}

/*Long version getBitsAt*/
long lgetBitsAt(long* n, int start, int size) {
	assert(size >= 0 && size <= (WORD_SIZE * 2));
	long mask = 0;

	for(int i = 0; i < size; ++i) {
		mask |= (1 << (start + i));
	}

	return (long)(((unsigned long)(*n & mask)) >> start);
}

void setBitsAt(int* n, int write_start, int* m, int read_start, int copy_size) {
	//Create a mask of copy_size 1s, so if size = 3, mask = 111
	int mask = 0;

	for(int i = 0; i < copy_size; ++i) {
		mask |= (1 << i);
	}

	*n &= ~(mask << write_start); //Clear the bits to be modified
	//Get the new_bits from m read_start position
	//Must cast to unsigned so it is logical right at the end (puts zeros)
	int new_bits = getBitsAt(m, read_start, copy_size);
	//Reshift the newbits to the write position and write
	*n |= (new_bits << write_start);
}

//Long version of setBitsAt
void lsetBitsAt(long* n, int w_start, long* m, int r_start, int copy_size) {
	//same stuff in long type
	long mask = 0;
	//1 in C is recognized as an int which crashed the shift below as it
	//overflows when copying more then 31 bits
	long one = 1;

	for(int i = 0; i < copy_size; ++i) {
		mask |= (one << i);
	}

	*n &= ~(mask << w_start); //Clear the bits to be modified
	//Get the new_bits from m r_start position
	long new_bits = lgetBitsAt(m, r_start, copy_size);
	//Reshift the newbits to the write position and write
	*n |= (new_bits << w_start);
}

/*Concatenates integer n to integer m to give a long */
long concatInts(int n, int m) {
	long msWord = (long)n << 32;
	//We need first unsigned cast not to get 1s in the upper 32 bits
	long lsWord = (long)((unsigned int)m);
	return msWord | lsWord;
}
