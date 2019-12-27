#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define KEY 24601
#define FNAME "telephone.txt"
/*
A main writing program
    Will attempt to access the resources using the semaphore
        Once in, it should display the last line added to the file (use shared memory to
        contain the size of the last line).
        Prompt the use for the next line.
        Once a new line is read, write that to the file, update the shared memory and then
        release the semaphore
You do not need a run target in your makefile
*/

int main(){
  //wait for semaphore to show that shared mem and file are free to touch
  printf("first seeing if semaphore is open...\n");
  int semd = semget(KEY, 1, 0);
  if (semd == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    exit(0);
  }
  int v = semctl(semd, 0, GETVAL, 0);
  printf("val of semaphore returned: %d\n", v);
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;
  semop(semd, &sb, 1);
  printf("it is, continuing\n");
  //
  //use shared mem to see length of last line, along with lseek and whence
  //printf("using shared memory to see length of last line...\n");
  int shmd = shmget(KEY, sizeof(int), 0);
  if (shmd == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    exit(0);
  }
  //printf("shmd: %d\n", shmd);
  //
  int * size_last = shmat(shmd, 0, 0);
  //printf("length of last line: %d\n", *size_last);
  //
  //display that last line (open, lseek, read, print, close)
  int fd = open(FNAME, O_RDONLY);
  //printf("moving cursor to show just last line...\n");
  if (lseek(fd, (*size_last * -1), SEEK_END) == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    exit(0);
  }
  //printf("moved cursor\n");
  //
  char to_print[*size_last];
  if (read(fd, to_print, *size_last) == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    exit(0);
  }
  printf("last addition: %s\n", to_print);
  //
  //printf("closing file...\n");
  if (close(fd) == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    exit(0);
  }
  //printf("closed\n");
  //
  //add new line (fgets, open w write + append, update shmem, write, close, release sem)
  printf("enter your addition:\n");
  char addition[200];
  if (fgets(addition, 200, stdin) == 0){
    printf("either error or you didn't input anything! Quitting...\n");
  }
  //
  fd = open(FNAME, O_WRONLY|O_APPEND);
  //update shmem
  //printf("updating shmem...\n");
  *size_last = strlen(addition);
  //printf("shmem updated\n");
  //write
  //printf("writing to %s...\n", FNAME);
  if (write(fd, addition, *size_last) == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    exit(0);
  }
  //printf("wrote\n");
  //close
  //printf("closing, detaching and releasing...\n");
  if (close(fd) == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    exit(0);
  }
  //detach shared memory
  shmdt(size_last);
  //release sem
  sb.sem_op = 1;
  semop(semd, &sb, 1);
  //
  //printf("done in writing\n");
  return 0;
}
