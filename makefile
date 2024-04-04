CC=gcc
INCDIRS=-I.
NOWARN=-Wno-error
#OPT=O0
CFLAGS=-Wall -Wextra -g $(INCDIRS) $(NOWARN) #$(OPT)

CFILES=main.c item.c utils.c
OBJECTS=main.o item.o utils.o

BINARY=bin 

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(BINARY) $(OBJECTS)
