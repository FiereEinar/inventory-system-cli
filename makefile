CC=gcc
INCDIRS=-I.
NOWARN=-Wno-error
#OPT=O0
CFLAGS=-Wall -Wextra -g $(INCDIRS) #$(NOWARN) #$(OPT)

CFILES=main.c item.c utils.c display.c sales.c storage.c
OBJECTS=main.o item.o utils.o display.o sales.o storage.o

BINARY=bin 

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	del $(OBJECTS)