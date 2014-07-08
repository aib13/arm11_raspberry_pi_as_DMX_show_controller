CC=gcc
CFLAGS=-Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic
LDFLAGS=
SOURCES=binread.c ../bitlib/bitlib.c printARM.c utils.c dataprocessing.c multiply.c single_data_transfer.c branch.c emulate.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=emulate

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f $(EXECUTABLE)
