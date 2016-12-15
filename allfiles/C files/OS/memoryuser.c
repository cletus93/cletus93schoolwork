//Author
//$Author: o1-rootz $
//
//Date
//$Date: 2016/12/11 16:42:03 $
//
//Log
//$Log: user.c,v $
//Revision 1.3  2016/12/11 16:42:03  o1-rootz
//added comments
//
//Revision 1.2  2016/12/11 06:53:20  o1-rootz
//added page requests
//
//Revision 1.1  2016/12/10 07:40:15  o1-rootz
//Initial revision
//
//
//
//Randall Rootz, 12/11/2016, this program is a user program run by the children in a process spawning file named memoryoss.c, it simulates a process requesting memory from the os and reading and writing from it 
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
#include "pages.h"

int sharedsec;    //global variable shared by all processes 
int sharednano;
int pageinfo;
int pagesid;
int requestid;
int shmkey = 29;
int shmkey2 = 122;
int shmkey3 = 123;
int shmkey4 = 321;
int shmkey5 = 987;
int *shmnanopointer;
int *shmsecpointer;
pages *infopointer;
int *pagespointer;
int *reqpointer;

int procnum;
int block;
int refnum = 0;
int nextcheck = 1000;
int numofpages;

volatile sig_atomic_t killSignal = 0;

void sigHandler(int);                        //prototypes
void recursivepart();

int main (int argc, char **argv)
{
   if(argc < 2)  //check for appropriate command line
   {
      printf("You need to enter all the correct data\n");
      printf("Please use the form,    execfile procnum PCBnum\n");
      exit(0);
   }
   procnum = atoi(argv[1]);      //number of this process
   block = atoi(argv[2]);        //process info block
   pid_t mypid = getpid();       //this processes pid

   signal(SIGINT, SIG_IGN);          //catch inturrupt
   signal(SIGQUIT, sigHandler);      //catch signal to quit

   if ((sharedsec= shmget(shmkey, 2, 0644 | IPC_CREAT)) == -1)   //attatch to shared memory
   {
      perror("shmget for sec failed in user");
      exit(1);
   }
   shmsecpointer = (int*) shmat(sharedsec, 0, 0);
   if ((sharednano = shmget(shmkey2, 4, 0644 | IPC_CREAT)) == -1)
   {
      perror("shmget for nano failed in user");
      exit(1);
   }
   shmnanopointer = (int*) shmat(sharednano, 0, 0);
   if ((pageinfo = shmget(shmkey3, 20 * sizeof(pages*), 0644 | IPC_CREAT)) == -1)
   {
      perror("shmget failed for pageinfor in user");
      exit(1);
   }
   infopointer = (pages*)shmat(pageinfo, 0, 0);
   if ((pagesid = shmget(shmkey4, 256 * sizeof(int*), 0644 | IPC_CREAT)) == -1)
   {
      perror("shmget failed for pages array in user");
      exit(1);
   }
   pagespointer = shmat(pagesid, (void *)0, 0);
   if ((requestid = shmget(shmkey3, 20 * sizeof(int*), 0644 | IPC_CREAT)) == -1)
   {
      perror("shmget failed for request array in user");
      exit(1);
   }
   reqpointer = (int*)shmat(requestid, 0, 0);

   if(killSignal == 1)          //do if child is killed
   {
      printf("Slave %d received quit signal.\n", procnum);
      exit(0);
   }

   time_t t;          //random time stuff
   srand((unsigned) time(&t));

   numofpages = rand() % 32 + 1;   //generate a random amount of pages to request up to 32
   infopointer[block].totalpages = numofpages;   //save how many pages this process will need
   reqpointer[block] = numofpages;    //request pages
 
   recursivepart();     //run critical section

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
   if (shmdt(infopointer) == -1)   
   {
      perror("shmdt for info in user");
      exit(1);
   }
   if (shmdt(pagespointer) == -1)
   {
      perror("shmdt for pages in user");
      exit(1);
   }
   if (shmdt(reqpointer) == -1)
   {
      perror("shmdt for request in user");
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

void recursivepart()      //critical section
{
   time_t t;          //random time stuff
   srand((unsigned) time(&t));

   if(pagespointer[infopointer[block].offset] == 0)   //check if the pages allocated to this process are still valid
   {
      reqpointer[block] = numofpages;  //put in a new request
   }
   while(1)   //wait for pages
   {
      if(reqpointer[block] == 0)  
      {
         break;
      }
   }
   
   int readorwrite = rand() % 2 + 1;   //randomly decide if the process will read or write
   if(readorwrite == 1)   //reading from page
   {
      infopointer[block].lasttimesec = *shmsecpointer;
      infopointer[block].lasttimenano = *shmnanopointer;
      refnum++;
   }
   else    //writing to page
   {
      infopointer[block].lasttimesec = *shmsecpointer;
      infopointer[block].lasttimenano = *shmnanopointer;
      refnum++;
   }
   if(refnum == nextcheck)  //check to exit program if only after one second
   {
      int checkexit = rand() % 2;
      if(checkexit == 1)   //exiting and freeing all memory and block info
      {
         int c;
         for(c = infopointer[block].offset; c < (infopointer[block].offset + infopointer[block].totalpages); c++)
         {
            pagespointer[c% 256] = 0;
         }
         infopointer[block].procnum = 0;
         infopointer[block].lasttimesec = 0;
         infopointer[block].lasttimenano = 0;
         infopointer[block].totalpages = 0;
         infopointer[block].offset = 0;
         reqpointer[block] = 0;
         exit(0);
      }
      else
      {
         int posneg = rand() % 2;   //decide if incr or decr
         if(posneg == 1)  //positive
         {
            int incrnum = rand() % 100 + 1;
            nextcheck = 1000 + incrnum;   //continue referencing
         }
         else
         {
            int incrnum = rand() % 100 + 1;
            nextcheck = 1000 - incrnum;   //continue referencing
         }
      }
   }
   recursivepart();
}
