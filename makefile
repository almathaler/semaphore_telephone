ifeq ($(DEBUG), true)
CC=gcc -g
else
CC=gcc
endif

all: control.o
	$(CC) -o control control.o
control.o: control.c control.h
	$(CC) -c control.c
clean:
	rm "control"
	rm *.o
