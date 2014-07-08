CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

all: assemble emulate

assemble:
	make -C assembler
	cp assembler/assemble .

emulate:
	make -C emulator
	cp emulator/emulate .

clean:
	rm -f $(wildcard *.o)
	rm assemble emulate
	make clean -C emulator
	make clean -C assembler

.PHONY: clean
