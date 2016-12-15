//Randall Rootz, 10/12/2016, this file was made to spawn child processes and calls a user process through the children, the children wait for the timer in oss.c to reach a certian point then exits and then the program spawns a new child, until 100 children finish or an amount of time passes, it also implements getopt
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <time.h>
#include "message.h"

void printstuff(void);             //all prototypes
void ChildProcess(int);
void ParentProcess(int);
void catch_alarm(int);
static void send_msg(int, int);
static const char * get_msg(int, int);

int sharedsec;                      //all shared memory values
int sharednano;
int shmkey = 29;
int shmkey2 = 122;
int *shmnanopointer;
int *shmsecpointer;

int keep_going;                         //all global values
int i, varx = 5, vary = 3, varz = 20;
char filename[100] = "test.out";
pid_t pids[101];
FILE *f;

int adultqid, kidsqid;
int msgtype = 1;
int adultmsgkey = 1234, kidsmsgkey = 4321;

int main (int argc, char **argv)
{
   int MAXSIZE = 0xFFF, fno, c, option_index = 0, n;
   char proclnk[0xFFF], str[150];
   ssize_t r;

   if(argc < 2)  //make sure there are options given
   {
      printf("You need to enter an option, try -h\n");
      exit(2);
   }
   adultqid = msgget(adultmsgkey, IPC_CREAT | 0666);   //initialize message boxes
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
   if ((sharedsec = shmget(shmkey, 2, 0644 | IPC_CREAT)) == -1)  //initialize shared memory
   {
      perror("shmget failed");
      exit(1);
   }
   if ((sharednano = shmget(shmkey2, 4, 0644 | IPC_CREAT)) == -1)
   {      perror("shmget failed");
      exit(1);
   }
   shmsecpointer = shmat(sharedsec, (void *)0, 0);
   shmnanopointer = shmat(sharednano, (void *)0, 0);
   if (shmsecpointer == (int *)(-1))
   {
      perror("shmat for sec in oss");
      exit(1);
   }
   if (shmnanopointer == (int *)(-1))
   {
      perror("shmat for nano in oss");
      exit(1);   }
   signal(SIGALRM, catch_alarm);       //tell program where to go for different signals
   signal(SIGINT, ParentProcess);

   while (1)    //work with getopt options
   {
      int this_option_optind = optind ? optind : 1;
      int option_index = 0;

   c = getopt (argc, argv, "hs:l:t:");
   if (c == -1)
      break;
   switch (c)
   {
      case 'h':
        printstuff();
        exit(-1);
      case 's':
        f = fopen (filename, "a+");
        if ( f == NULL)
        {
           perror("Can't open file\n");           exit(-1);
        }
        varx = atoi(optarg);
        break;
      case 'l':
        f = fopen (argv[2], "a+");
        if ( f == NULL)
        {
           perror("Can't open file\n");
           exit(-1);
        }
        fno = fileno(f);
        sprintf(proclnk, "/proc/self/fd/%d", fno);
        r = readlink(proclnk, filename, MAXSIZE);
        if (r < 0)
        {
           perror("failed to readlink\n");
           exit(1);
        }
        filename[r] = '\0';
        break;
      case 't':
        f = fopen (filename, "a+");
        if ( f == NULL)
        {
           perror("Can't open file\n");
           exit(-1);
        }
        varz = atoi(optarg);
        break;
      case '?':
        if (optopt == 'c')
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
         else if (isprint (optopt))
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);
         else
            fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
      default:
        abort();
      }
   }
   for (option_index = optind; option_index < argc; option_index++)
      printf ("Non-option argument %s\n", argv[option_index]);

   int status, processcount = 0, totalrunproc = 0;
   pid_t pid, wpid;

   *shmsecpointer = 0;    //set timer to 0.0
   *shmnanopointer = 0;
   keep_going = varz * 40000; //set timer to keep track of real time max sec(default 20)
   ualarm(25 , 25);   //call sig_alarm every 25 microseconds

   send_msg(kidsqid, msgtype);   //send message to user.c to start kids off
   while(1)
   {
      if(processcount < varx)  //only spawn a max of varx processes at one time
      {
         processcount++;
         i++;   //keep track of total number of processes
         pids[i] = fork();  //fork and store all pids of children
         if (pids[i] == 0)
         {
            ChildProcess(i);
         }
         else
         {

            wpid = wait(&status);  //wait to get and exit by user.c            
			processcount--;        //decrement to allow room for one more process to be created            
			const char* resultstring = get_msg(adultqid, msgtype);  //recieve time message from user.c            
			fprintf(f, "Master: Child %d is terminating at my time %d.%d because it reached %s\n", wpid, *shmsecpointer, *shmnanopointer, resultstring);
            send_msg(kidsqid, msgtype);    //send message to keep kids going
            if(i == 100)  //only 100 kids can finish
            {
               printf("You reached 100 finished processes\n");
               break;
            }
            else if(*shmsecpointer == 2) //only 2 simulated seconds can pass
            {
               printf("Your reached 2 seconds of simulated time\n");
               printf("Number of finished processes: %d\n", i);
               break;
            }
         }
      }
   }
   ParentProcess(1);  //kill all kids if it has not been done   fclose(f);     //close file
   return 0;       //end main}
void ParentProcess(int s)
{
   signal(SIGQUIT, SIG_IGN);  //ignore all signals and interrupts
   signal(SIGALRM, SIG_IGN);
   signal(SIGINT, SIG_IGN);
   if (shmdt(shmsecpointer) == -1)   //disconnect from shared memory and deallocate
   {
      perror("shmdt for sec in oss");
      exit(1);
   }
   if (shmdt(shmnanopointer) == -1)
   {
      perror("shmdt for nano in oss");
      exit(1);
   }

   shmctl(sharedsec, IPC_RMID, 0);
   shmctl(sharednano, IPC_RMID, 0);
   msgctl(adultqid, IPC_RMID, 0);     //deallocate message boxes
   msgctl(kidsqid, IPC_RMID, 0);

   if(s == SIGINT)   //print if CTRL-C is caught
   {
      fprintf(stderr, "\nReceived CTRL-C. Killing all child processes\n");
      printf("Number of finished process: %d\n", i);
   }   else if(s == SIGALRM)   //print if varz seconds is reached
   {      fprintf(stderr, "Reached %d seconds. Killing all child processes\n", varz);      printf("Number of finished process: %d\n", i);   //I put this in so you do not have to manually count the number of processes in the file
   }
   else
   {      fprintf(stderr, "Program finished\n");   //this is the end of any operations besides return and fclose
   }
   for(i = 0; i < varx; i++)
   {      kill(pids[i], SIGTERM);   //now that everything is deallocated we can make sure all kids are killed
   }

}
void  ChildProcess(int i)
{
   char userprocnum[100];
   char totalprocnum[100];
   sprintf(userprocnum, "%d", i);   sprintf(totalprocnum, "%d", varx);
   execlp("user", "user", userprocnum, totalprocnum, (char *) 0);
}

void printstuff(void)    //list of possible options for when -h is recieved
{   printf("The command line arguments avialable are:\n");
   printf("-h          Used to display all command line args\n");   printf("-s x        Used to change the number of children processes being made to the value x\n");   printf("-l filename Used to change the name of the log file being written to\n");   printf("-t z        Used to change the time the oss process will wait until killing all processes to the value z\n");
}
void catch_alarm(int sig)   //ualarm is complicated it increments 40,000 times by 25 microseconds, which equals 1 second and above this one second is multiplied by varz to go get the number of times alarm will be decrement keep_going to reach varz seconds
{
   keep_going--;   
   *shmnanopointer += 6439;    //random number to increment simulated time by, every 25 microseconds   
   if(*shmnanopointer > 1000000000) //this is math to make sure when 1000000000 is reached it returns to 0 and seconds is incremented
   {
      *shmnanopointer = *shmnanopointer - 1000000000;
      *shmsecpointer += 1;
   }   
   if(keep_going == 0)     //catch if varz seconds is reached
   {      
      ParentProcess(SIGALRM);  //run ParentProcess with SIGALRM so it knows varz seconds was reached   
   }
}

static void send_msg(int qid, int msgtype)
{   
   struct msgbuf msg;  //create structure   
   msg.mtype = msgtype;
   snprintf(msg.mtext, sizeof(msg.mtext), "your turn");   //copy a message to be put in a box
   if (msgsnd(qid, (void *) &msg, sizeof(msg.mtext), IPC_NOWAIT) == -1)   //send a message that does no wait
   {
      perror("msgsnd error");   //error check if it fails
      exit(EXIT_FAILURE);
   }
}

static const char * get_msg(int qid, int msgtype)
{
   struct msgbuf msg;     //create structure
   static char resultstring[100];
   if (msgrcv(qid, (void *) &msg, sizeof(msg.mtext), msgtype, MSG_NOERROR) == -1)    //recieve a message from queue and wait if there is none
   {
      if (errno != ENOMSG)
      {
         perror("msgrcv");   //error out if fail         
		 exit(EXIT_FAILURE);
      }
      printf("No message available for msgrcv()\n");   }
   else
   {      sprintf(resultstring, "%s", msg.mtext);     //print message to a string pointer
   }
   return resultstring;   //return that pointer to be used by master
}
