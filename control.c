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
#include <sys/stat.h>
#include "control.h"

#define KEY 24601
#define FNAME "telephone.txt"

// union semun {
//   int              val;    /* Value for SETVAL */
//   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
//   unsigned short  *array;  /* Array for GETALL, SETALL */
//   struct seminfo  *__buf;  /* Buffer for IPC_INFO
//                               (Linux-specific) */
// };

int main(int argc, char * argv[]){
  if (argc < 2){
    printf("You haven't selected c, v or r! Quitting...\n");
    exit(0);
  }
  char * cmd_line = argv[1]; //first arg is name of program
  int to_do = crv(cmd_line);
  switch (to_do) {
    case 0:
      //printf("you've selected CREATE:\n");
      if (c()){
        printf("Something went wrong! In CREATE\n");
      }
      break;
    case 1:
      //printf("you've selected REMOVE:\n");
      if (r()){
        printf("Something went wrong! In REMOVE\n");
      }
      break;
    case 2:
      //printf("you've selected VIEW:\n");
      if (v()){
        printf("Something went wrong! In VIEW\n");
      }
      break;
    default:
      printf("You haven't selected c, v or r! Quitting...\n");
      exit(0);
  }
  return 0;
}

int crv(char* cmd_line){
  if (strchr(cmd_line, 'c')){
    return 0;
  }else if(strchr(cmd_line, 'r')){
    return 1;
  }else if(strchr(cmd_line, 'v')){
    return 2;
  }else{
    return 3;
  }
}

int c(){
  printf("NOTE: you may not create twice in a row!\n");

  //printf("creating file...\n");
  int fd;
  fd = open(FNAME, O_CREAT|O_EXCL|O_TRUNC, 0666);
  if (fd == -1){
    printf("You Trickster! You've already done \"./control -c\"! Quitting...\n");
    exit(0);
  }
  printf("\"telephone.txt\" created...\n");

  //printf("creating shared memory...\n");
  //NOTE: TO GET LAST LINE, MAKE SIZE OF LAST LINE IN SHARED MEM AND WHEN PRINTING USE LSEEK TO MOVE CURSOR
  int shmd;
  shmd = shmget(KEY, sizeof(int), IPC_CREAT|0644);
  if (shmd == -1){
    printf("error: %d: %s\n", errno, strerror(errno));
  }
  //printf("shmd: %d\n", shmd);
  //set initial val to 0
  //printf("setting shared mem initial val to 0...\n");
  int *set_zero = shmat(shmd, 0, 0);
  *set_zero = 0;
  //printf("set shared mem intial val to: %d\n", *set_zero);
  shmdt(set_zero);
  printf("shared memory created\n");
  //
  //this created it, do similar in write so you can view what was previously written
  //printf("creating semaphore...\n");
  int semd;
  int v, r;
  semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  if (semd == -1) {
    printf("error %d: %s\n", errno, strerror(errno));
    semd = semget(KEY, 1, 0);
    v = semctl(semd, 0, GETVAL, 0);
    //printf("got value of existing semaphore, semctl returned: %d\n", v);
  }
  else {
    union semun us;
    us.val = 1;
    r = semctl(semd, 0, SETVAL, us);
    //printf("set value of our semaphore to %d, semctl returned: %d\n", us.val, r);
  }
  printf("created semaphore, done creating\n");
  return 0;
}

int r(){
  //printf("in remove\n");
  v(); //so you can see what's there so far
  printf("\n");
  printf("first seeing if semaphore is open...\n");
  int semd = semget(KEY, 1, 0);
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;
  semop(semd, &sb, 1);
  printf("it is, continuing\n");
  //
  //printf("removing semaphore...\n");
  if (semd == -1){
    printf("error %d: %s\n", errno, strerror(errno));
  }
  if(semctl(semd, IPC_RMID, 0)){
    printf("error %d: %s\n", errno, strerror(errno));
  }
  printf("removed semaphore\n");
  //
  //printf("removing shared memory...\n");
  int shmd = shmget(KEY, sizeof(int), 0);
  if (shmctl(shmd, IPC_RMID, 0)){
    printf("error %d: %s\n", errno, strerror(errno));
  }
  printf("removed shared mem\n");
  //
  //printf("removing the file...\n");
  if (remove(FNAME)){
    printf("error %d: %s\n", errno, strerror(errno));
  }
  printf("removed the file\n");
  //
  return 0;
}

int v(){
  printf("The story so far:\n");
  //to decide how much to read from, get size of file
  struct stat file_stat;
  if (stat(FNAME, &file_stat)){
    printf("you've encountered an error! perhaps you've done ./control -r or have not yet done ./control -c\n");
    exit(0);
  }
  int size = file_stat.st_size;
  //now read in then printf
  if (size == 0){
    printf("\n");
    return 0;
  }
  char to_print[size];
  int fd = open(FNAME, O_RDONLY);
  int check = read(fd, to_print, size);
  if (check != size){
    printf("For some reason, didn't read whole file\n");
  }
  printf("%s", to_print);
  return 0;
}
