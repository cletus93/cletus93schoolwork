//Author
//$Author: o1-rootz $
//
//Date
//$Date: 2016/12/11 16:41:50 $
//
//Log
//$Log: oss.c,v $
//Revision 1.3  2016/12/11 16:41:50  o1-rootz
//added comments
//
//Revision 1.2  2016/12/11 06:52:55  o1-rootz
//added pages
//
//Revision 1.1  2016/12/10 07:39:57  o1-rootz
//Initial revision
//
//
//
//Randall Rootz, 12/11/2016, this file was made to spawn child processes every random amount of time and calls a user process through the children, from there the children simulate processes requesting memory from the cpu 
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
#include "pages.h"

void printstuff(void);   //prototypes
void ChildProcess(int, int);
void ParentProcess(int);
void nextrun();
void moveclock();
void allocpages();
int findmin(int, int);
void makespace();
void printtables();

int sharedsec;    //shared memory variables
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

//global variables
int i, varx = 8, vary = 3, varz = 20, processcount = 0, finishedproc = 0, myflag, totalwrite = 0, nextrunsec, nextrunnano, nextrunincr, j, verbose = 0, freepages = 0;
char filename[100] = "test.out";
pid_t pids[101];
FILE *f;

int main (int argc, char **argv)
{
   int MAXSIZE = 0xFFF, fno, c, option_index = 0, n, k;
   char proclnk[0xFFF], str[150];
   ssize_t r;    //random generator stuff
   time_t t;
  
   srand((unsigned) time(&t));
  
   nextrunsec = 0;   //set time to 0.0
   nextrunsec = 0;

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
   }
   if ((pageinfo = shmget(shmkey3, 18 * sizeof(pages*), 0644 | IPC_CREAT)) == -1)
   {
      perror("shmget failed for pageinfo in oss");
      exit(1);
   }
   if ((pagesid = shmget(shmkey4, 256 * sizeof(int*), 0644 | IPC_CREAT)) == -1)
   {
      perror("shmget failed for pages array in oss");
      exit(1);
   }
   if ((requestid = shmget(shmkey5, 20 * sizeof(int*), 0644 | IPC_CREAT)) == -1)
   {
      perror("shmget failed for request array in oss");
      exit(1);
   }
   shmsecpointer = shmat(sharedsec, (void *)0, 0);  //pointers to shared memory
   shmnanopointer = shmat(sharednano, (void *)0, 0);
   infopointer = (pages*)shmat(pageinfo, 0, 0);
   pagespointer = shmat(pagesid, (void *)0, 0);
   reqpointer = shmat(requestid, (void *)0, 0);
   if (shmsecpointer == (int *)(-1))
   {
      perror("shmat for sec in oss");
      exit(1);
   }
   if (shmnanopointer == (int *)(-1))
   {
      perror("shmat for nano in oss");
      exit(1);
   }
   if (infopointer == (pages*)(-1))
   {
      perror("shmat for pageinfo in oss");
      exit(1);
   }
   if (pagespointer == (int *)(-1))
   {
      perror("shmat for pages array in oss");
      exit(1);
   }
   if (reqpointer == (int *)(-1))
   {
      perror("shmat for request array in oss");
      exit(1);
   }

   signal(SIGALRM, ParentProcess);  //catch alarm
   signal(SIGINT, ParentProcess);   //catch inturrupt

   while (1)  //all get opt
   {
      int this_option_optind = optind ? optind : 1;
      int option_index = 0;

   c = getopt (argc, argv, "hs:l:t:v:");
   if (c == -1)
      break;
   switch (c)
   {
      case 'h':     //list of all command line options
        printstuff();
        exit(-1);
      case 's':     //change the number of concurrent processes, can't exceed 18
        f = fopen (filename, "a+");
        if ( f == NULL)
        {
           perror("Can't open file\n");
           exit(-1);
        }
        varx = atoi(optarg);
        if (varx > 18)
        {
           printf("Cant't run more that 18 children at a time, please run agian with a number lower than 19\n");
           exit(-1);
        }
        break;
      case 'l':   //change the name of the log file
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
      case 't':   //change the amount of time to wait before killing program
        f = fopen (filename, "a+");
        if ( f == NULL)
        {
           perror("Can't open file\n");
           exit(-1);
        }
        varz = atoi(optarg);
        break;
      case 'v':     //change output to verbose or not
            f = fopen (filename, "a+");
            if ( f == NULL)
            {
               perror("Can't open file\n");
               exit(-1);
            }
            verbose = atoi(optarg);
            if (verbose > 1)
            {
               printf("Cant't set verbose can only be set to 0 or 1, rerun program please\n");
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

   for(k = 0; k < 20; k++)
   {
      pagespointer[k] = 0;
   }
   *shmsecpointer = 0;   //set sim time to 0.0
   *shmnanopointer = 0;
   int currentsec = 0;   //set time to print page tables
   nextrunsec = 0;   //set time to try to spawn next child to 0.0
   nextrunnano = 0;
   i = 0;

   alarm(varz);   //call alarm to go off after varz seconds

   while(1)
   {
      int status;
      pid_t pid, wpid;
      
      if(*shmsecpointer > currentsec) //run table print every simulated second
      {
         currentsec += 1;   //count seconds for table print to run
         fprintf(f, "OSS: Printing page tables at time %d.%d\n", *shmsecpointer, *shmnanopointer);
         fflush(f);
         printtables();  //call page table print 
      }
      if(*shmsecpointer > nextrunsec)   //time to run agian
      {
         nextrun();   //get next time to try to spawn child
         allocpages();   //alloc memory to a process
         freepages = 0;   //keep track of free memory
         for(i = 0; i < 256; i++)
         {
            if(pagespointer[i] != -1)
            {
               freepages++;   //count free pages
            }
         }
         if(freepages < 25)  //less than 10 percent of memory is free
         {
            makespace();   //free all memory
         }
         if(processcount < varx)   //no more concurrent processes that varx
         {
            processcount++;   //number of concurrent processes
            i++;     //process id number
            pids[i] = fork();  //fork child
            if (pids[i] == 0)
            {
               pid = getpid();   //get child pid
               ChildProcess(i, j);
            }
            else
            {
               wpid = wait(&status);
               if (wpid == -1) {            /* error calling waitpid       */
                  perror("waitpid error");
                  exit(EXIT_FAILURE);
               }
               finishedproc += 1; //number of finished processes
               processcount--;  //allow another process to spawn
               if(i == 30)   //only let 100 processes finish
               {
                  printf("You reached 100 finished processes\n");
                  break;
               }
            }
         }
      }
      else if(*shmsecpointer >= nextrunsec && *shmnanopointer >= nextrunnano)   // time to run agian
      { 
         nextrun();   //get time time to spawn a child
         allocpages();   //alloc memory to a process
         freepages = 0;   //keep track of free memory
         for(i = 0; i < 256; i++)
         {
            if(pagespointer[i] != -1)
            {
               freepages++;   //count free pages
            }
         }
         if(freepages < 25)  //less than 10 percent of memory is free
         {
            makespace();   //free all memory
         }
         if(processcount < varx)  //dont make more than varx children at a time
         {
            processcount++;   //concurrent process count
            i++;           //process id number
            pids[i] = fork();  //fork child
            if (pids[i] == 0)
            {
               pid = getpid();  //get child pid
               ChildProcess(i, j);
            }
            else
            {
               wpid = wait(&status);
               if (wpid == -1) {            /* error calling waitpid       */
                  perror("waitpid error");
                  exit(EXIT_FAILURE);
               }
               finishedproc += 1;
               processcount--;
               if(i == 30)
               {
                  printf("You reached 100 finished processes\n");
                  break;
               }
            }
         }
      }
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
   signal(SIGQUIT, SIG_IGN);   //ignore all signals
   signal(SIGALRM, SIG_IGN);
   signal(SIGINT, SIG_IGN);

   if (shmdt(shmsecpointer) == -1)   //detach from shared memory
   {
      perror("shmdt for sec in oss");
      exit(1);
   }
   if (shmdt(shmnanopointer) == -1)
   {
      perror("shmdt for nano in oss");
      exit(1);
   }
   if (shmdt(infopointer) == -1)
   {
      perror("shmdt for pageinfo in oss");
      exit(1);
   }
   if (shmdt(pagespointer) == -1)
   {
      perror("shmdt for page array in oss");
      exit(1);
   }
   if (shmdt(reqpointer) == -1)
   {
      perror("shmdt for request array in oss");
      exit(1);
   }

   shmctl(sharedsec, IPC_RMID, 0);    //deallocate shared memory
   shmctl(sharednano, IPC_RMID, 0);
   shmctl(pageinfo, IPC_RMID, 0);
   shmctl(pagesid, IPC_RMID, 0);
   shmctl(requestid, IPC_RMID, 0);

   if(s == SIGINT)
   {
      fprintf(stderr, "\nReceived CTRL-C. Killing all child processes\n");
      printf("Number of finished process: %d\n", finishedproc);
   }
   else if(s == SIGALRM)
   {      fprintf(stderr, "Reached %d seconds. Killing all child processes\n", varz);
      printf("Number of finished process: %d\n", finishedproc);
   }
   else
   {
      fprintf(stderr, "Program finished\n");
   }
   for(i = 0; i < varx; i++)
   {
      kill(pids[i], SIGTERM);   //kill all processes
   }

}

void  ChildProcess(int i, int j)   //spawned child runs user.c
{
   int k;
   char userprocnum[100];
   sprintf(userprocnum, "%d", i);

   for(k = 0; k < 20; k++)   //find next empty process control block
   {
      if(infopointer[k].totalpages == 0)
      {
         break;
      }
   }
   infopointer[k].procnum = i;  //assign process to the empty block
   char userk[100];
   sprintf(userk, "%d", k);   //tell the user.c which block is thiers

   execlp("user", "user", userprocnum, (char *) 0);
}

void printstuff(void)   //list of all command line options for user with descriptions
{
   printf("The command line arguments avialable are:\n");
   printf("-h          Used to display all command line args\n");
   printf("-s x        Used to change the number of children processes being made concurrently to the value x\n");
   printf("-l filename Used to change the name of the log file being written to\n");
   printf("-t z        Used to change the time the oss process will wait until killing all processes to the value z\n");
   printf("-v w        Used to change the output printed to the logfile, 0 for everything and 1 for verbose\n");
}

void nextrun()   //get random amount of time to next try and spawn a child
{
   nextrunincr = rand() % 2000000001;
   if((nextrunnano + nextrunincr) > 1000000000)
   {
      nextrunnano = (nextrunnano + nextrunincr) % 1000000000;   //dont let adding rand number make nano exceed 1000000000
      nextrunsec = nextrunsec + 1;
   }
   else
   {
      nextrunnano = nextrunnano + nextrunincr;
      nextrunsec = nextrunsec;
   }
}

void moveclock()   //increment the simulated clock
{
   int clockincr = rand() % 1001;
   *shmnanopointer += clockincr;
   if(*shmnanopointer > 1000000000)
   {
      *shmnanopointer = *shmnanopointer - 1000000000;
      *shmsecpointer += 1;
   }
}

void allocpages()   //allocate a page of memory to a process when requested
{
   int i, n, m, nogood = 0, p, l;
   for(i = 0; i < 20; i++)    //check all requests
   {
      if(reqpointer[i] != 0)  //if there is a request handle it
      {
         break;
      }
   }
   
   for(n = 0; n < 256; n++)  //check if there is available space, must be continuous
   {
      nogood = 0;
      if(pagespointer[n] == 0)   //found one empty page
      {
         for(m = 0; m < reqpointer[i]; m++)   //check if there are enough continuous pages to fill the order, if not keep checking
         {
            if(pagespointer[(m + n) % 256] != 0)
            {
               nogood++;
            }
         }
         if(nogood == 0)    //found room for process in pages
         {
            infopointer[i].offset = n;
            for(p = n; p < n + reqpointer[i]; p++)
            {
               pagespointer[p % 256] = 1;  //allocate pages to the process
            }
            reqpointer[i] = 0;
            break;
         }
      }
   }
   if(nogood != 0)    //page fault detected
   {
      int makeroom[20];
      for(l = 0; l < 20; l++)
      {
         makeroom[l] = -1;
      }
      if(verbose == 0)
      {
         fprintf(f, "OSS: Detected a page fault at time %d:%d\n", *shmsecpointer, *shmnanopointer);
      }
      for(n = 0; n < 20; n++)  //find all processes with space allocated that have enough for this process to replace one of them
      {
         if(infopointer[i].totalpages <= infopointer[n].totalpages)
         {
            for(l = 0; l < 20; l++)
            {
               if(makeroom[l] == -1)
               {
                  makeroom[l] = n;  //add process to array of processes
                  break;
               }
            }
         }
      }
      int toclear = makeroom[0];
      for(n = 1; n < 20; n++)   //find the least recently used process
      {
         toclear = findmin(toclear, makeroom[n]);
      }
      if(verbose == 0)
      {
         fprintf(f, "OSS: Replacing process %d to at time %d:%d\n", toclear, *shmsecpointer, *shmnanopointer);
      }
      for(m = infopointer[toclear].offset; m < (infopointer[toclear].offset + infopointer[toclear].totalpages); m++)
      {
         pagespointer[m % 256] = 0;   //free memory from the LRU process
      }
   }
}

int findmin(int x, int y)  //compare the times the processes were used last return the oldest
{
   if(infopointer[x].lasttimesec < infopointer[y].lasttimesec)
   {
      return x;
   }
   else if(infopointer[x].lasttimenano < infopointer[y].lasttimenano)
   {
      return x;
   }
   else
   {
      return y;
   }
}

void makespace()  //free memory from LRU process
{
   int toclear = 0, n, m;
   if(verbose == 0)
   {
      fprintf(f, "OSS: Detected memory usage above 90 percent at time %d:%d\n", *shmsecpointer, *shmnanopointer);
   }
   for(n = 1; n < 20; n++)
   {
      if(infopointer[n].totalpages)
      {
         toclear = findmin(toclear, n);  //find LRU process
      }
   }
   if(verbose == 0)
   {
      fprintf(f, "OSS: Clearing up memory for process %d at time %d:%d\n", toclear, *shmsecpointer, *shmnanopointer);
   }
   for(m = infopointer[toclear].offset; m < (infopointer[toclear].offset + infopointer[toclear].totalpages) % 256; m++)
   {
      pagespointer[m] = 0;   //free pages
   }
}

void printtables()   //print the page tables to the log file
{
   char pagetable[256][10];  //list of pages
   int n;
   for(i = 0; i < 256; i++)
   {
      sprintf(pagetable[i], ".");  //mark all pages as free
   }
   for(i = 0; i < 20; i++)  //mark all pages that have a process with that process number
   {
      if(infopointer[i].totalpages != 0)
      {
         for(n = infopointer[i].offset; n <(infopointer[i].offset + infopointer[i].totalpages); n++);
         {
            sprintf(pagetable[n % 256], "process %d", infopointer[i].procnum);
         } 
      }
   }
   fprintf(f, "_________\n");   //format line for view in log file
   for(i = 0; i < 256; i++)
   {
      fprintf(f, "|%s|\n", pagetable[i]);   //print page tables
   }
   fprintf(f, "________\n");    //format line for view in log file
}
