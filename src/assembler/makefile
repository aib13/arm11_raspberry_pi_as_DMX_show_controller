CC=gcc
CFLAGS=-Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic
SOURCES=../bitlib/bitlib.c dataprocessing.c multiply.c branch.c single_data_transfer.c special.c binary_writer.c hash.c text_reader.c tokeniser.c labeliser.c utils.c assemble.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=assemble

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f $(EXECUTABLE)
