all: main.o rawMode.o strcmp.o
	gcc -o program main.o rawMode.o strcmp.o

main.o: main.c
	gcc -c main.c

rawMode.o: rawMode.c
	gcc -c rawMode.c

strcmp.o: strcmp.c
	gcc -c strcmp.c

run:
	./program
