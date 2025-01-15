CFLAGS = -Wall -Wextra -g -std=c99
CC = gcc

all: main.o functs.o
	$(CC) -o main main.o functs.o

functs.o: functs.c functs.h
	$(CC) -c $(CFLAGS) functs.c

main.o: main.c functs.h
	$(CC) -c $(CFLAGS) main.c

clean:
	rm -f *.o *~ tp3
