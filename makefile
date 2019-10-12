CC=gcc
CFLAGS= -g -Wall

EXE = NFA

OBJ0 = NFA.o

all: $(EXE) clear

NFA: $(OBJ0)
	$(CC) $(CFLAGS) -o NFA $(OBJ0)

NFA.o: NFA.c NFA.h
	$(CC) $(CFLAGS) -c NFA.c

clean:
	rm -rf $(OBJ0) *~ $(EXE)

clear:
	rm -rf $(OBJ0)
