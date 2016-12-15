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
#include "resource.h"

void printstuff(void);   //prototypes
void ChildProcess(int, int);
void ParentProcess(int);
void nextrun();void moveclock();void giveresource();
void deadlockcheck();
int max(int, int);

int sharedsec;    //shared memory variables
int sharednano;
int resourceid;
int requestid;
int shmkey = 29;
int shmkey2 = 122;
int shmkey3 = 394;
int shmkey4 = 543;
int *shmnanopointer;
int *shmsecpointer;
resource *sharedrec;
int *sharedrequest;

//global variablesint i, varx = 5, vary = 3, varz = 20, verbose = 0, processcount = 0, finishedproc = 0, myflag, totalwrite = 0, currentsec, currentnano, nextrunsec, nextrunnano, nextrunincr, j, k;
char filename[100] = "test.out";
pid_t pids[101];
FILE *f;

int main (int argc, char **argv)
{
   int MAXSIZE = 0xFFF, fno, c, option_index = 0, n, k;
   char proclnk[0xFFF], str[150];
   ssize_t r;    //random generator stuff
   time_t t;

   int resallocarray[19];
   for(i = 0; i < 20; i++)
   {
      resallocarray[i] = 0;
   }

   srand((unsigned) time(&t));
   nextrunsec = 0;   //set time to 0.0   nextrunsec = 0;

   if(argc < 2)   //make sure to get conmand option
   {
      printf("You need to enter an option, try -h\n");
      exit(2);
   }
   if ((sharedsec = shmget(shmkey, 2, 0644 | IPC_CREAT)) == -1)  //connect to shared memory
   {
      perror("shmget failed for sec in oss");
      exit(1);
   }
   if ((sharednano = shmget(shmkey2, 4, 0644 | IPC_CREAT)) == -1)
   {
      perror("shmget failed for nano in oss");
      exit(1);
   }   if ((resourceid = shmget(shmkey3, 20 * sizeof(resource*), 0644 | IPC_CREAT)) == -1)
   {
      perror("shmget failed for resources in oss");
      exit(1);
   }
   if ((requestid = shmget(shmkey4, 20 * sizeof(int*), 0644 | IPC_CREAT)) == -1)
   {
      perror("shmget failed for request array in oss");
      exit(1);
   }   shmsecpointer = shmat(sharedsec, (void *)0, 0);  //pointers to shared memory
   shmnanopointer = shmat(sharednano, (void *)0, 0);
   sharedrec = (resource*)shmat(resourceid, 0, 0);
   sharedrequest = shmat(requestid, (void *)0, 0);
   if (shmsecpointer == (int *)(-1))
   {
      perror("shmat for sec in oss");
      exit(1);
   }
   if (shmnanopointer == (int *)(-1))
   {      perror("shmat for nano in oss");
      exit(1);
   }
   if (sharedrec == (resource*)(-1))
   {
      perror("shmat for resources in oss");
      exit(1);
   }
   if (sharedrequest == (int *)(-1))
   {
      perror("shmat for request array in oss");
      exit(1);
   }


   for(i = 0; i < 20; i++)   //clear out all process control blocks
   {
      sharedrec[i].resourcenum = 1;
      sharedrec[i].instnum = 0;
      sharedrec[i].allocarray.inst1 = -1;
      sharedrec[i].allocarray.inst2 = -1;
      sharedrec[i].allocarray.inst3 = -1;
      sharedrec[i].allocarray.inst4 = -1;
      sharedrec[i].allocarray.inst5 = -1;
      sharedrec[i].allocarray.inst6 = -1;
      sharedrec[i].allocarray.inst7 = -1;
      sharedrec[i].allocarray.inst8 = -1;
      sharedrec[i].allocarray.inst9 = -1;
      sharedrec[i].allocarray.inst10 = -1;
   }   sharedrec[2].resourcenum = 3;   //These are all the resources with multiple instances, at this point any resource can be changed to any number of instances from 1 to 10
   sharedrec[6].resourcenum = 5;
   sharedrec[11].resourcenum = 7;
   sharedrec[17].resourcenum = 9;

   for(i = 0; i < 20; i++)   //clear out all process control blocks
   {
      sharedrequest[i] = -1;
   }


   signal(SIGALRM, ParentProcess);  //catch alarm
   signal(SIGINT, ParentProcess);   //catch inturrupt

   while (1)  //all get opt
   {
      int this_option_optind = optind ? optind : 1;
      int option_index = 0;

      c = getopt (argc, argv, "hs:l:t:v:");      if (c == -1)         break;
      switch (c)
      {
         case 'h':
            printstuff();
            exit(-1);
         case 's':
            f = fopen (filename, "a+");
            if ( f == NULL)
            {
               perror("Can't open file\n");
               exit(-1);
            }
            varx = atoi(optarg);
            if (varx > 18)
            {               printf("Cant't run more that 18 children at a time, please run agian with a number lower than 19\n");
               exit(-1);
            }
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
            break;         case 't':
            f = fopen (filename, "a+");
            if ( f == NULL)
            {
               perror("Can't open file\n");
               exit(-1);
            }
            varz = atoi(optarg);
            break;
           case 'v':
            f = fopen (filename, "a+");
            if ( f == NULL)
            {
               perror("Can't open file\n");
               exit(-1);
            }
            verbose = atoi(optarg);
            if (verbose > 1)
            {               printf("Cant't set verbose can only be set to 0 or 1, rerun program please\n");
               exit(-1);
            }
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

   *shmsecpointer = 0;   //set sim time to 0.0
   *shmnanopointer = 0;
   nextrunsec = 0;   //set time to try to spawn next child to 0.0   nextrunnano = 0;
   currentsec = 0;
   currentnano = 0;
   i = 0;

   alarm(varz);   //call alarm to go off after varz seconds

   while(1)
   {
      int status;
      pid_t pid, wpid;

      if(*shmsecpointer > currentsec)
      {
         currentsec += 1;         printf("OSS: Using deadlock detection at time %d.%d\n", *shmsecpointer, *shmnanopointer);         fprintf(f, "OSS: Using deadlock detection at time %d.%d\n", *shmsecpointer, *shmnanopointer);
         deadlockcheck();
      }

      giveresource();
      if(*shmsecpointer > nextrunsec)
      {
         nextrun();   //get next time to try to spawn child
         if(processcount < varx)   //no more concurent processes that varx
         {
            for(k = 0; k < 20; k++)
            {
               if(sharedrequest[k] != -1)
               {
                  break;
               }
            }
            processcount++;   //number of concurrent processes
            i++;     //process id number            pids[i] = fork();  //fork child
            if (pids[i] == 0)            {
               pid = getpid();   //get child pid
               if(verbose == 0)
               {                  printf("OSS: Generating process with PID %d at time %d:%d\n", pid, *shmsecpointer, *shmnanopointer);                  fprintf(f, "OSS: Generating process with PID %d at time %d:%d\n", pid, *shmsecpointer, *shmnanopointer);
               }
               ChildProcess(i, j);
            }
            else
            {
               wpid = wait(&status);
               if(verbose == 0)
               {
                  printf("OSS: Recieving process with PID %d\n", wpid);
                  fprintf(f, "OSS: Recieving process with PID %d\n", wpid);
               }
               processcount--;  //allow another process to spawn
               if(i == 10000)   //only let 100 processes finish
               {
                  printf("You reached 10000 finished processes\n");
                  break;
               }
            }         }
      }      else if(*shmsecpointer >= nextrunsec && *shmnanopointer >= nextrunnano)
      {
         nextrun();   //get time to spawn a child
         if(processcount < varx)  //dont make more than varx children at a time
         {
            for(k = 0; k < 20; k++)
            {
               if(sharedrequest[k] != -1)
               {
                  break;
               }
            }
            processcount++;   //concurrent process count
            i++;           //process id number
            pids[i] = fork();  //fork child
            if (pids[i] == 0)
            {
               pid = getpid();  //get child pid
               if(verbose == 0)
               {                  printf("OSS: Generating process with PID %d at time %d:%d\n", pid, *shmsecpointer, *shmnanopointer);                  fprintf(f, "OSS: Generating process with PID %d at time %d:%d\n", pid, *shmsecpointer, *shmnanopointer);
               }
               ChildProcess(i, j);
            }
            else
            {
               wpid = wait(&status);
               if(verbose == 0)
               {
                  printf("OSS: Recieving process with PID %d\n", wpid);
                  fprintf(f, "OSS: Recieving process with PID %d\n", wpid);
               }
               finishedproc += 1;
               processcount--;
               if(i == 10000)
               {
                  printf("You reached 10000 finished processes\n");
                  break;
               }            }
         }      }
      else
      {
         nextrun();  //get new time to try and spawn a child
      }
      moveclock();//increment simulated clock
   }

   ParentProcess(1);
   fclose(f);
   return 0;
}
void ParentProcess(int s)   //kill all children, detach from shared mem and messages, and kill self
{
   signal(SIGQUIT, SIG_IGN);
   signal(SIGALRM, SIG_IGN);
   signal(SIGINT, SIG_IGN);

   if (shmdt(shmsecpointer) == -1)
   {
      perror("shmdt for sec in oss");
      exit(1);
   }
   if (shmdt(shmnanopointer) == -1)
   {
      perror("shmdt for nano in oss");
      exit(1);
   }
   if (shmdt(sharedrec) == -1)
   {
      perror("shmdt for resources in oss");
      exit(1);
   }   if (shmdt(sharedrequest) == -1)
   {
      perror("shmdt for request array in oss");
      exit(1);
   }

   shmctl(sharedsec, IPC_RMID, 0);
   shmctl(sharednano, IPC_RMID, 0);
   shmctl(resourceid, IPC_RMID, 0);
   shmctl(requestid, IPC_RMID, 0);

   if(s == SIGINT)
   {
      fprintf(stderr, "\nReceived CTRL-C. Killing all child processes\n");
      printf("Number of finished process: %d\n", finishedproc);
   }
   else if(s == SIGALRM)   {      fprintf(stderr, "Reached %d seconds. Killing all child processes\n", varz);
      printf("Number of finished process: %d\n", finishedproc);
   }
   else
   {
      fprintf(stderr, "Program finished\n");
   }
   for(i = 0; i < varx; i++)
   {
      kill(pids[i], SIGTERM);
   }

}

void  ChildProcess(int i, int j)   //spawned child runs user.c
{
   char userprocnum[100];
   sprintf(userprocnum, "%d", i);
   char bound[100] = "10000";
   char userk[100];
   sprintf(userk, "%d", k);   execlp("user", "user", userprocnum, bound, userk, (char *) 0);
}
void printstuff(void)   //list of all command line options for user with descriptions
{
   printf("The command line arguments avialable are:\n");
   printf("-h          Used to display all command line args\n");   printf("-s x        Used to change the number of children processes being made to the value x\n");   printf("-l filename Used to change the name of the log file being written to\n");   printf("-t z        Used to change the time the oss process will wait until killing all processes to the value z\n");   printf("-v k        Used to change output to the file, 0 is write everything, 1 is write only deadlock info\n");
}

void nextrun()   //get random amount of time to next try and spawn a child
{
   nextrunincr = rand() % 501;
   if((nextrunnano + nextrunincr) > 1000000000)
   {      nextrunnano = (nextrunnano + nextrunincr) % 1000000000;   //dont let adding rand number make nano exceed 1000000000      nextrunsec = nextrunsec + 1;
   }
   else
   {
      nextrunnano = nextrunnano + nextrunincr;      nextrunsec = nextrunsec;
   }}
void moveclock()   //increment the simulated clock
{   int clockincr = rand() % 100000001;
   *shmnanopointer += clockincr;
   if(*shmnanopointer > 1000000000)
   {
      *shmnanopointer = *shmnanopointer - 1000000000;
      *shmsecpointer += 1;
   }
}
void giveresource()
{
   for(i = 0; i < 20; i++)
   {
      if(sharedrequest[i] > -1)
      {
         if(sharedrec[sharedrequest[i]].allocarray.inst1 == -1)
         {
            sharedrec[sharedrequest[i]].allocarray.inst1 = sharedrequest[i];
            sharedrequest[i] = -1;
         }
         if(sharedrec[sharedrequest[i]].resourcenum > 1)
         {
            if(sharedrec[sharedrequest[i]].allocarray.inst2 == -1)
            {
               sharedrec[sharedrequest[i]].allocarray.inst2 = sharedrequest[i];
               sharedrequest[i] = -1;
            }
         }
         if(sharedrec[sharedrequest[i]].resourcenum > 2)
         {
            if(sharedrec[sharedrequest[i]].allocarray.inst3 == -1)
            {
               sharedrec[sharedrequest[i]].allocarray.inst3 = sharedrequest[i];
               sharedrequest[i] = -1;
            }
         }
         if(sharedrec[sharedrequest[i]].resourcenum > 3)
         {
            if(sharedrec[sharedrequest[i]].allocarray.inst4 == -1)
            {
               sharedrec[sharedrequest[i]].allocarray.inst4 = sharedrequest[i];
               sharedrequest[i] = -1;
            }
         }
         if(sharedrec[sharedrequest[i]].resourcenum > 4)
         {
            if(sharedrec[sharedrequest[i]].allocarray.inst5 == -1)
            {
               sharedrec[sharedrequest[i]].allocarray.inst5 = sharedrequest[i];
               sharedrequest[i] = -1;
            }
         }
         if(sharedrec[sharedrequest[i]].resourcenum > 5)
         {
            if(sharedrec[sharedrequest[i]].allocarray.inst6 == -1)
            {
               sharedrec[sharedrequest[i]].allocarray.inst6 = sharedrequest[i];
               sharedrequest[i] = -1;
            }
         }
         if(sharedrec[sharedrequest[i]].resourcenum > 6)
         {
            if(sharedrec[sharedrequest[i]].allocarray.inst7 == -1)
            {
               sharedrec[sharedrequest[i]].allocarray.inst7 = sharedrequest[i];
               sharedrequest[i] = -1;
            }
         }
         if(sharedrec[sharedrequest[i]].resourcenum > 7)
         {
            if(sharedrec[sharedrequest[i]].allocarray.inst8 == -1)
            {
               sharedrec[sharedrequest[i]].allocarray.inst8 = sharedrequest[i];
               sharedrequest[i] = -1;
            }
         }
         if(sharedrec[sharedrequest[i]].resourcenum > 8)
         {
            if(sharedrec[sharedrequest[i]].allocarray.inst9 == -1)
            {
               sharedrec[sharedrequest[i]].allocarray.inst9 = sharedrequest[i];
               sharedrequest[i] = -1;
            }
         }
         if(sharedrec[sharedrequest[i]].resourcenum > 9)
         {
            if(sharedrec[sharedrequest[i]].allocarray.inst10 == -1)
            {
               sharedrec[sharedrequest[i]].allocarray.inst10 = sharedrequest[i];
               sharedrequest[i] = -1;
            }
         }
      }
   }
}

void deadlockcheck()
{
   int resource[20];
   int avialable[20];
   int askedfor[varx][20];
   int alloc[varx][20];
   int markarray[varx];
   int canfinish = 1;
   int mymaxarray[varx];

   for(i = 0; i < varx; i++)
   {
       markarray[i] = 0;
       for(j = 0; j < 20; j++)
       {
          alloc[i][j] = 0;
          askedfor[i][j] = 0;
       }
   }

   for(i = 0; i < 20; i++)
   {
      resource[i] = sharedrec[i].resourcenum;
   }

   for(i = 0; i < 20; i++)
   {
      if(sharedrec[i].allocarray.inst1 != -1)
      {
         alloc[sharedrec[i].allocarray.inst1][i] += 1;
      }
      if(sharedrec[i].allocarray.inst2 != -1)
      {
         alloc[sharedrec[i].allocarray.inst1][i] += 1;
      }
      if(sharedrec[i].allocarray.inst3 != -1)
      {
         alloc[sharedrec[i].allocarray.inst1][i] += 1;
      }
      if(sharedrec[i].allocarray.inst4 != -1)
      {
         alloc[sharedrec[i].allocarray.inst1][i] += 1;
      }
      if(sharedrec[i].allocarray.inst5 != -1)
      {
         alloc[sharedrec[i].allocarray.inst1][i] += 1;
      }
      if(sharedrec[i].allocarray.inst6 != -1)
      {
         alloc[sharedrec[i].allocarray.inst1][i] += 1;
      }
      if(sharedrec[i].allocarray.inst7 != -1)
      {
         alloc[sharedrec[i].allocarray.inst1][i] += 1;
      }
      if(sharedrec[i].allocarray.inst8 != -1)
      {
         alloc[sharedrec[i].allocarray.inst1][i] += 1;
      }
      if(sharedrec[i].allocarray.inst9 != -1)
      {
         alloc[sharedrec[i].allocarray.inst1][i] += 1;
      }
      if(sharedrec[i].allocarray.inst10 != -1)
      {
         alloc[sharedrec[i].allocarray.inst1][i] += 1;
      }
   }

   for(i = 0; i < 20; i++)
   {
      avialable[i] = resource[i];
   }

   for(i = 0; i < varx; i++)
   {
      for(j = 0; j < 20; j++)
      {
         avialable[i] -= alloc[i][j];
      }
   }

   for(i = 0; i < 20; i++)
   {
      askedfor[sharedrequest[i]][i] += 1;
   }

   for(i = 0; i < varx; i++)
   {
      for(j = 0; j < 20; j++)
      {
         if(askedfor[i][j] != -1)
         {
            break;
         }
         markarray[i] = 1;
      }
   }

   while(canfinish == 1)
   {
      canfinish = 0;
      for(i = 0; i < varx; i++)
      {
         if(markarray[i] == 1)
         {
            for(j = 0; j < 20; j++)
            {
               if(askedfor[i][j] > avialable[j])
               {
                  markarray[i] = 1;
                  break;
               }
               else
               {
                  markarray[1] = 0;
               }
            }
            if(markarray == 0)
            {
               canfinish = 1;
               for(j = 0; j < 20; j++)
               {
                  avialable[i] += alloc[i][j];
               }
            }
         }
      }
   }

   int mydeadlockcheck = 0;
   for(i = 0; i < varx; i++)
   {
      if(markarray[i] == 1)
      {
         mydeadlockcheck += 1;
      }
   }
   if(mydeadlockcheck != 0)
   {      printf("OSS: Detected a deadlock at time %d:%d\n", *shmsecpointer, *shmnanopointer);      fprintf(f, "OSS: Detected a deadlock at time %d:%d\n", *shmsecpointer, *shmnanopointer);
      for(i = 0; i < varx; i++)
      {
         mymaxarray[i] = 0;
      }
      for(i = 0; i < varx; i++)
      {
         for(j = 0; j < 20; j++)
         {
            mymaxarray[i] += alloc[i][j];
         }
      }
      int maxalloc;
      for(i = 0; i < varx; i++)
      {
         maxalloc = max(mymaxarray[i], mymaxarray[i++]);
      }      printf("OSS: Removing allocated resources from process %d at time %d:%d\n", maxalloc, *shmsecpointer, *shmnanopointer);      fprintf(f, "OSS: Removing allocated resources from process %d at time %d:%d\n", maxalloc, *shmsecpointer, *shmnanopointer);      for(i = 0; i < 20; i++)
      {
         alloc[maxalloc][i] = 0;
         if(sharedrec[i].allocarray.inst10 == maxalloc)
         {
            sharedrec[i].allocarray.inst10 = -1;            printf("OSS: Releasing resource %d from process %d at time %d:%d\n", i,  maxalloc, *shmsecpointer, *shmnanopointer);            fprintf(f, "OSS: Releasing resource %d from process %d at time %d:%d\n", i, maxalloc, *shmsecpointer, *shmnanopointer);
         }
         if(sharedrec[i].allocarray.inst9 == maxalloc)
         {
            sharedrec[i].allocarray.inst9 = -1;            printf("OSS: Releasing resource %d from process %d at time %d:%d\n", i,  maxalloc, *shmsecpointer, *shmnanopointer);            fprintf(f, "OSS: Releasing resource %d from process %d at time %d:%d\n", i, maxalloc, *shmsecpointer, *shmnanopointer);         }
         if(sharedrec[i].allocarray.inst8 == maxalloc)         {
            sharedrec[i].allocarray.inst8 = -1;            printf("OSS: Releasing resource %d from process %d at time %d:%d\n", i,  maxalloc, *shmsecpointer, *shmnanopointer);            fprintf(f, "OSS: Releasing resource %d from process %d at time %d:%d\n", i, maxalloc, *shmsecpointer, *shmnanopointer);
         }
         if(sharedrec[i].allocarray.inst7 == maxalloc)         {
            sharedrec[i].allocarray.inst7 = -1;            printf("OSS: Releasing resource %d from process %d at time %d:%d\n", i,  maxalloc, *shmsecpointer, *shmnanopointer);            fprintf(f, "OSS: Releasing resource %d from process %d at time %d:%d\n", i, maxalloc, *shmsecpointer, *shmnanopointer);
         }
         if(sharedrec[i].allocarray.inst6 == maxalloc)         {
            sharedrec[i].allocarray.inst6 = -1;            printf("OSS: Releasing resource %d from process %d at time %d:%d\n", i,  maxalloc, *shmsecpointer, *shmnanopointer);            fprintf(f, "OSS: Releasing resource %d from process %d at time %d:%d\n", i, maxalloc, *shmsecpointer, *shmnanopointer);
         }
         if(sharedrec[i].allocarray.inst5 == maxalloc)         {
            sharedrec[i].allocarray.inst5 = -1;            printf("OSS: Releasing resource %d from process %d at time %d:%d\n", i,  maxalloc, *shmsecpointer, *shmnanopointer);            fprintf(f, "OSS: Releasing resource %d from process %d at time %d:%d\n", i, maxalloc, *shmsecpointer, *shmnanopointer);
         }
         if(sharedrec[i].allocarray.inst4 == maxalloc)         {
            sharedrec[i].allocarray.inst4 = -1;            printf("OSS: Releasing resource %d from process %d at time %d:%d\n", i,  maxalloc, *shmsecpointer, *shmnanopointer);            fprintf(f, "OSS: Releasing resource %d from process %d at time %d:%d\n", i, maxalloc, *shmsecpointer, *shmnanopointer);
         }
         if(sharedrec[i].allocarray.inst3 == maxalloc)         {
            sharedrec[i].allocarray.inst3 = -1;            printf("OSS: Releasing resource %d from process %d at time %d:%d\n", i,  maxalloc, *shmsecpointer, *shmnanopointer);            fprintf(f, "OSS: Releasing resource %d from process %d at time %d:%d\n", i, maxalloc, *shmsecpointer, *shmnanopointer);
         }
         if(sharedrec[i].allocarray.inst2 == maxalloc)         {
            sharedrec[i].allocarray.inst2 = -1;            printf("OSS: Releasing resource %d from process %d at time %d:%d\n", i,  maxalloc, *shmsecpointer, *shmnanopointer);            fprintf(f, "OSS: Releasing resource %d from process %d at time %d:%d\n", i, maxalloc, *shmsecpointer, *shmnanopointer);
         }
         if(sharedrec[i].allocarray.inst1 == maxalloc)         {
            sharedrec[i].allocarray.inst1 = -1;            printf("OSS: Releasing resource %d from process %d at time %d:%d\n", i,  maxalloc, *shmsecpointer, *shmnanopointer);            fprintf(f, "OSS: Releasing resource %d from process %d at time %d:%d\n", i, maxalloc, *shmsecpointer, *shmnanopointer);
         }
      }      printf("OSS: Recheck deadlock at time %d:%d\n", *shmsecpointer, *shmnanopointer);      fprintf(f, "OSS: Recheck deadlock at time %d:%d\n", *shmsecpointer, *shmnanopointer);
      deadlockcheck();
   }
   else
   {      printf("OSS: Check proved no deadlock at time %d:%d\n", *shmsecpointer, *shmnanopointer);      fprintf(f, "OSS: Check proved no deadlock at time %d:%d\n", *shmsecpointer, *shmnanopointer);
   }
}

int max(int x, int y){
   if(x >= y)   {
      return x;
   }
   else   {
      return y;   }
}
