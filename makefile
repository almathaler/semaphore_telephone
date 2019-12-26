ifeq ($(DEBUG), true)
CC=gcc -g
else
CC=gcc
endif

all: control.o write.o
	$(CC) -o control control.o
	$(CC) -o write write.o
write.o: write.c
	$(CC) -c write.c
control.o: control.c control.h
	$(CC) -c control.c
clean:
	rm "control"
	rm "write"
	rm *.o
