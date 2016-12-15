//Randall Rootz, 10/12/2016, this program is a user program run by the children in a process spawning file named oss.c, it waits for the timer in shared memory to reach a certian value then exits
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
#include "message.h"

int sharedsec;     //all shared memory values
int sharednano;
int shmkey = 29;
int shmkey2 = 122;
int *shmsecpointer;
int *shmnanopointer;

volatile sig_atomic_t killSignal = 0;    //killsignal

void critical_section(int, int, int, int, int, int);  //prototypes
void sigHandler(int);
static void send_msgtokids(int, int);
static void send_msgtoadult(int, int);
static const char * get_msg(int, int);

int main (int argc, char **argv)
{
   if(argc < 3)  //check for appropriate command line
   {
      printf("You need to enter all the correct data\n");
      printf("Please use the form,    execfile processnum totalprocnum\n");
      exit(0);
   }
   int startsec, startnano, endsec, endnano;  //all global variables
   int procnum = atoi(argv[1]);
   int varx = atoi(argv[1]);
   pid_t mypid = getpid();

   int adultqid, kidsqid;
   int msgtype = 1;
   int adultmsgkey = 1234, kidsmsgkey = 4321;
   
   signal(SIGINT, SIG_IGN);    //what to do with signals, ignore inturrupt, sent quit to sigHandler
   signal(SIGQUIT, sigHandler);
   
   adultqid = msgget(adultmsgkey, IPC_CREAT | 0666);   //connect to two mailboxes
   if (adultqid == -1)
   {
      perror("msgget failed for adult in oss");
      exit(EXIT_FAILURE);
   }
   kidsqid = msgget(kidsmsgkey, IPC_CREAT | 0666);
   if (kidsqid == -1)
   {
      perror("msgget failed for kids in oss");
      exit(EXIT_FAILURE);
   }
   if ((sharedsec= shmget(shmkey, 2, 0644 | IPC_CREAT)) == -1)  //connect to shared memory
   {
      perror("shmget for sec failed in user");      exit(1);
   }
   shmsecpointer = (int*) shmat(sharedsec, 0, 0);
   if ((sharednano= shmget(shmkey2, 4, 0644 | IPC_CREAT)) == -1)   {
      perror("shmget for nano failed in user");
      exit(1);
   }
   shmnanopointer = (int*) shmat(sharednano, 0, 0);

   startsec = *shmsecpointer;   //use sim time to get time to look for with math
   startnano = *shmnanopointer;

   time_t t;
   srand((unsigned) time(&t));   //random number stuff

   int randincr = (rand() % 100000);   //get rand number
   if((startnano + randincr) > 1000000000)
   {      endnano = (startnano + randincr) % 1000000000;   //dont let adding rand number make nano exceed 1000000000
      endsec = startsec + 1;
   }
   else
   {
      endnano = startnano + randincr;
      endsec = startsec;
   }   critical_section(procnum, endsec, endnano, adultqid, kidsqid, msgtype); //start critical section
   
   kill(mypid, SIGTERM);   //tell master your finished
   kill(mypid, SIGKILL);
   return 0;
}
void critical_section(int procnum, int endsec, int endnano, int adultqid, int kidsqid, int msgtype)
{   
   const char* turnstring = get_msg(kidsqid, msgtype);  //only continue if you can get a message from kids mailbox
   if(killSignal == 1)   {
      printf("Slave %d received quit signal.\n", procnum);
      exit(0);
      //break;
   }
   usleep(30);  //sleep to allow for incrementing in between
   if(*shmsecpointer > endsec)
   {
      send_msgtoadult(adultqid, msgtype);  //if endtime exceeded send current time to oss
      exit(0);
   }
   else if(*shmsecpointer >= endsec && *shmnanopointer >= endnano)
   {
	   send_msgtoadult(adultqid, msgtype);    //if endtime exceeded send current time to oss      exit(0);
   }
   else   
   {      
      send_msgtokids(kidsqid, msgtype);   //if endtime not exceeded send message to allow another child to check
      critical_section(procnum, endsec, endnano, adultqid, kidsqid, msgtype);
   }

   if (shmdt(shmsecpointer) == -1)  //disconnect from shared memory
   {
      perror("shmdt for sec in user");
      exit(1);
   }   if (shmdt(shmnanopointer) == -1)
   {
      perror("shmdt for nano in user");
      exit(1);
   }
}

void sigHandler(int s)   //switch kill signal
{
  killSignal = 1;
}
static void send_msgtokids(int qid, int msgtype)   //operations to send messages to kids mailbox
{
   struct msgbuf msg;   //create structure for writing
   msg.mtype = msgtype;

   snprintf(msg.mtext, sizeof(msg.mtext), "your turn");  //message to send
   if (msgsnd(qid, (void *) &msg, sizeof(msg.mtext), 0) == -1)   //send message and wait for someone to recieve it
   {
      perror("msgsnd error");
      exit(EXIT_FAILURE);
   }
}
static void send_msgtoadult(int qid, int msgtype)    //operations to send messages to adult mailbox
{
   struct msgbuf msg;   //create structure
   msg.mtype = msgtype;
   snprintf(msg.mtext, sizeof(msg.mtext), "%d.%d", *shmsecpointer, *shmnanopointer);    //message of current time to send to oss
   if (msgsnd(qid, (void *) &msg, sizeof(msg.mtext), 0) == -1)  //send message and wait for someone to recieve it
   {
      perror("msgsnd error");
      exit(EXIT_FAILURE);   
   }
}
static const char * get_msg(int qid, int msgtype)   //gets message form specified mailbox
{
   struct msgbuf msg;  //create structure
   static char turnstring[100];
   if (msgrcv(qid, (void *) &msg, sizeof(msg.mtext), msgtype, MSG_NOERROR) == -1)   //get message from queue and wait if there is none
   {
      if (errno != ENOMSG)
      {         
         perror("msgrcv");
         exit(EXIT_FAILURE);
      }   
   }
   else
   {
      sprintf(turnstring, "%s", msg.mtext);  //print message to string pointer
   }
   return turnstring;   //return pointer to be used by user.c if needed
}

