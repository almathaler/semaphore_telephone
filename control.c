#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

#include "control.h"

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
  printf("in create\n");
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
