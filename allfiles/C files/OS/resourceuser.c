#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <signal.h>
#include "resource.h"

int sharedsec;    //global variable shared by all processes
int sharednano;
int resourceid;
int requestid;
int shmkey = 29;
int shmkey2 = 122;
int shmkey3 = 394;
int shmkey4 = 543;
int *shmsecpointer;
int *shmnanopointer;
resource *sharedrec;
int *sharedrequest;

int procnum;
int bound;
int arraynum;
int myresources[10];
int mytimesec, mytimenano, nexttimesec, nexttimenano;
char *filename;

volatile sig_atomic_t killSignal = 0;

void sigHandler(int);                        //prototypes
void recursivepart(int);

int main (int argc, char **argv)
{
   if(argc < 4)  //check for appropriate command line
   {
      printf("You need to enter all the correct data\n");
      printf("Please use the form,    execfile procnum bound arraynum\n");
      exit(0);
   }
   procnum = atoi(argv[1]);      //number of this process
   bound = atoi(argv[2]);
   arraynum = atoi(argv[3]);
   pid_t mypid = getpid();

   int i;
   for(i = 0; i < 10; i++)
   {
      myresources[i] = 0;
   }

   signal(SIGINT, SIG_IGN);          //catch inturrupt
   signal(SIGQUIT, sigHandler);      //catch signal to quit

   if ((sharedsec= shmget(shmkey, 2, 0644 | IPC_CREAT)) == -1)   //attatch to shared memory
   {
      perror("shmget for sec failed in user");
      exit(1);
   }
   shmsecpointer = (int*) shmat(sharedsec, 0, 0);
   if ((sharednano= shmget(shmkey2, 4, 0644 | IPC_CREAT)) == -1)
   {
      perror("shmget for nano failed in user");
      exit(1);
   }
   shmnanopointer = (int*) shmat(sharednano, 0, 0);
   if ((resourceid = shmget(shmkey3, 20 * sizeof(resource*), 0644 | IPC_CREAT)) == -1)
   {
      perror("shmget failed for resources in user");
      exit(1);
   }
   sharedrec = (resource*)shmat(resourceid, 0, 0);
   if ((requestid = shmget(shmkey4, 20 * sizeof(int*), 0644 | IPC_CREAT)) == -1)
   {
      perror("shmget failed for request array in oss");
      exit(1);
   }
   sharedrequest = shmat(requestid, (void *)0, 0);

   if(killSignal == 1)          //do if child is killed
   {
      printf("Slave %d received quit signal.\n", procnum);
      exit(0);
   }
   mytimesec = *shmsecpointer + 1;
   mytimenano = *shmnanopointer;
   int mybound = bound;
   recursivepart(mybound);     //run critical section

   if (shmdt(shmsecpointer) == -1)   //detatch shared mem
   {
      perror("shmdt for sec in user");
      exit(1);
   }
   if (shmdt(shmnanopointer) == -1)
   {
      perror("shmdt for nano in user");
      exit(1);
   }
   if (shmdt(sharedrec) == -1)
   {
      perror("shmdt for resources in user");
      exit(1);
   }

   kill(mypid, SIGTERM);    //kill self
   kill(mypid, SIGKILL);
   return 0;
}

void sigHandler(int s)   //set kill signal variable in inturrupt if caught
{
  killSignal = 1;
}

void recursivepart(int mybound)
{
   time_t t;
   srand((unsigned) time(&t));
   while(mybound != 0)
   {
      int decbound = rand() % mybound + 1;
      mybound = mybound - decbound;
      *shmnanopointer += 6439;
      if(*shmnanopointer > 1000000000)
      {
         *shmnanopointer = *shmnanopointer - 1000000000;
         *shmsecpointer += 1;
      }
   }
   int decide = rand() % 2;
   if(decide == 1)
   {
      int pickresource = rand() % 20;
      sharedrequest[arraynum] = pickresource;
      while(sharedrequest[arraynum] != -1)
      {
         //wait for resource
      }
      myresources[pickresource] = myresources[pickresource] + 1;
   }
   else
   {
      int resourcecount, i;
      for(i = 0; i < 20; i++)
      {
         if(myresources[i] != 0)
         {
            resourcecount = resourcecount + myresources[i];
         }
      }
      int randnum = rand() % resourcecount;
      resourcecount = 0;
      for(i = 0; i < 20; i++)
      {
         if(myresources[i] != 0)
         {
            resourcecount = resourcecount + myresources[i];
         }
         if(resourcecount == randnum)
         {
            break;
         }
      }
      myresources[i] = myresources[i] - 1;
      if(sharedrec[i].allocarray.inst10 == procnum)
      {
          sharedrec[i].allocarray.inst10 = -1;
      }
      else if(sharedrec[i].allocarray.inst9 == procnum)
      {
          sharedrec[i].allocarray.inst9 = -1;
      }
      else if(sharedrec[i].allocarray.inst8 == procnum)
      {
          sharedrec[i].allocarray.inst8 = -1;
      }
      else if(sharedrec[i].allocarray.inst7 == procnum)
      {
          sharedrec[i].allocarray.inst7 = -1;
      }
      else if(sharedrec[i].allocarray.inst6 == procnum)
      {
          sharedrec[i].allocarray.inst6 = -1;
      }
      else if(sharedrec[i].allocarray.inst5 == procnum)
      {
          sharedrec[i].allocarray.inst5 = -1;
      }
      else if(sharedrec[i].allocarray.inst4 == procnum)
      {
          sharedrec[i].allocarray.inst4 = -1;
      }
      else if(sharedrec[i].allocarray.inst3 == procnum)
      {
          sharedrec[i].allocarray.inst3 = -1;
      }
      else if(sharedrec[i].allocarray.inst2 == procnum)
      {
          sharedrec[i].allocarray.inst2 = -1;
      }
      else if(sharedrec[i].allocarray.inst1 == procnum)
      {
          sharedrec[i].allocarray.inst1 = -1;
      }
      else
      {
      }
   }
   if(*shmsecpointer >= mytimesec && *shmnanopointer >= mytimenano)
   {
      int checkexit = rand() % 2;
      if(checkexit == 1)
      {
         exit(0);
      }
      else
      {
         int nextcheck = rand() % 251;
         mytimenano = nextcheck;
         if(mytimenano > 1000000000)
         {
            mytimesec += 1;
            mytimenano -= 1000000000;
         }
      }
   }
   mybound = bound;
   recursivepart(mybound);
}
