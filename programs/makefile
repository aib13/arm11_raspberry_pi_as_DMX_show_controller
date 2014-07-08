CC=gcc
CFLAGS=-Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

all: kernel.img

kernel.img: binrw dmx.img show.bin
	./$< dmx.img show.bin $@

binrw: binrw.o

dmx.img: dmx.s
	make -C ../src
	../src/assemble $< $@

show.bin: show_creator show.shw
	./$< show.shw $@

show_creator: show_creator.o

clean:
	rm -f *.o
	rm show_creator
	rm binrw
	rm kernel.img
	rm dmx.img
	rm show.bin

.PHONY: clean
