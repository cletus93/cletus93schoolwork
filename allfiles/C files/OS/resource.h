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

typedef struct allocresource
{
   int inst1;
   int inst2;
   int inst3;
   int inst4;
   int inst5;
   int inst6;
   int inst7;
   int inst8;
   int inst9;
   int inst10;
}allocresource;

typedef struct resource
{
   int resourcenum;
   int instnum;
   allocresource allocarray;
}resource;
