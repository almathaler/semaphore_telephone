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
#include "control.h"

#define KEY 24601

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
                              (Linux-specific) */
};

int main(int argc, char * argv[]){
  char * cmd_line = argv[1]; //first arg is name of program
  int to_do = crv(cmd_line);
  switch (to_do) {
    case 0:
      printf("you've selected CREATE:\n");
      if (c()){
        printf("Something went wrong! In CREATE\n");
      }
      break;
    case 1:
      printf("you've selected REMOVE:\n");
      if (r()){
        printf("Something went wrong! In REMOVE\n");
      }
      break;
    case 2:
      printf("you've selected VIEW:\n");
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
  printf("in create -- NOTE: you may not create twice in a row!\n");

  printf("creating file...\n");
  int fd;
  fd = open("telephone.txt", O_CREAT|O_EXCL|O_TRUNC, 0666);
  if (fd == -1){
    printf("You Trickster! You've already done \"./control -c\"! Quitting...\n");
    exit(0);
  }
  printf("\"telephone.txt\" created...\n");

  printf("creating shared memory...\n");
  //NOTE: TO GET LAST LINE, MAKE SIZE OF LAST LINE IN SHARED MEM AND WHEN PRINTING USE LSEEK TO MOVE CURSOR
  int shmd;
  shmd = shmget(KEY, sizeof(int), IPC_CREAT|0644); //made it, don't really need to touch it
  //this created it, do similar in write so you can view what was previously written
  printf("creating semaphore...\n");

  return 0;
}

int r(){
  printf("in remove\n");
  return 0;
}

int v(){
  printf("in view\n");
  return 0;
}
