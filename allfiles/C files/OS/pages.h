//Randall Rootz, 12/11/2016, this header file contains the structs for the pages in oss.c and user.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <getopt.h>
#include <ctype.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

typedef struct pages 
{
   int procnum;
   int totalpages;
   int offset;
   int lasttimesec;
   int lasttimenano;
}pages;

