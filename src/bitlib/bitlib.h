
//Defined in bitlib.c
int getBitAt(int* n, int index);
int lgetBitAt(long* n, int index);
void setBitAt(int* n, int index, int new_digit);
int getBitsAt(int* n, int start, int size);
long lgetBitsAt(long* n, int start, int size);
void setBitsAt(int* n, int write_start, int* m, int read_start, int copy_size);
void lsetBitsAt(long* n, int w_start, long* m, int r_start, int copy_size);
long concatInts(int n, int m);
