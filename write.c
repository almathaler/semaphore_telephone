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
