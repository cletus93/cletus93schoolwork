//Randall Rootz, 9/29/2016, this file was made to spawn child processes and calla slave process through the children, it also implements getopt
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
#include <sys/shm.h>

void printstuff(void);     //function prototypes
void ChildProcess(int);
void ParentProcess(int);

int sharedNum;         //shared mem globals
int shmkey = 122;
int *shmpointer;

int i, varx = 5, vary = 3, varz = 20;    //global variables
char filename[100] = "test.out";
pid_t pids[20];

int main (int argc, char **argv)
{
   int MAXSIZE = 0xFFF, fno, c, option_index = 0, n;
   char proclnk[0xFFF];
   ssize_t r;

   FILE *f;    //file pointer

   signal(SIGALRM, ParentProcess);   //make signals to kill all processes
   signal(SIGINT, ParentProcess);

   if(argc < 2)   //make sure there is a command line option
   {
      printf("You need to enter an option, try -h\n");
      exit(2);
   }

   if ((sharedNum = shmget(shmkey, 10, 0644 | IPC_CREAT)) == -1)   //allocate shared mem
   {
      perror("shmget failed");
      exit(1);
   }
   shmpointer = shmat(sharedNum, (void *)0, 0);  //attach to shared mem
   if (shmpointer == (int *)(-1))
   {
      perror("shmat");
      exit(1);
   }

   while (1)     //handle command line options
   {
      int this_option_optind = optind ? optind : 1;
      int option_index = 0;

   c = getopt (argc, argv, "hs:l:i:t:");
   if (c == -1)
      break;
   switch (c)
   {
      case 'h':
        printstuff();
        exit(-1);
      case 's':
        varx = atoi(optarg);
        break;
      case 'l':
        f = fopen (argv[2], "a+");
        if ( f == NULL)
        {
           perror("Can't open file\n");
           exit(-1);
        }
        fprintf(f, "hello");
        fno = fileno(f);
        sprintf(proclnk, "/proc/self/fd/%d", fno);
        r = readlink(proclnk, filename, MAXSIZE);
        if (r < 0)
        {
           perror("failed to readlink\n");
           exit(1);
        }
        filename[r] = '\0';
        fclose(f);
        break;
      case 'i':
        vary = atoi(optarg);
        break;
      case 't':
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

   int status, processcount = 0;
   pid_t pid;

   alarm(varz);

   for (i = 1; i <= varx; i++)
   {
      processcount++;
      pids[i] = fork();
      if (pids[i] == 0)
      {
         printf("Starting process %d\n", i);
         ChildProcess(i);
         printf("Finished process %d\n", i);
         exit(0);
      }
      else
      {
         while(processcount != 0)
         {
            wait(&status);
            processcount--;
         }
      }
   }
   if (shmdt(shmpointer) == -1)    //dettach from shared mem
   {
      perror("shmdt failed, master");
      exit(1);
   }

   shmctl(sharedNum, IPC_RMID, 0);   //deallocate shared mem
   return 0;
}
void ParentProcess(int s)
{
   signal(SIGQUIT, SIG_IGN);   //ignore quit signal
   signal(SIGALRM, SIG_IGN);   //ignore alarm signal
   signal(SIGINT, SIG_IGN);    //ignore inturrupt signal
   if(s == SIGINT)
   {
      fprintf(stderr, "\nReceived CTRL-C. Killing all child processes\n");
   }
   else if(s == SIGALRM)
   {
      fprintf(stderr, "Timer ran out. Killing all child processes\n");
   }
   for(i = 0; i < varx; i++)
   {
      kill(pids[i], SIGTERM);
   }

}
void  ChildProcess(int i)   //convert data to strings and pass to slave as command line args
{
   char slavevary[100];
   char slaveprocnum[100];
   char slavevarx[100];
   sprintf(slavevary, "%d", vary);
   sprintf(slaveprocnum, "%d", i);
   sprintf(slavevarx, "%d", varx);

   execlp("slave", "slave", slaveprocnum, slavevary, filename, slavevarx, (char *) 0);
}

void printstuff(void)  //print all help of command options
{
   printf("The command line arguments avialable are:\n");
   printf("-h          Used to display all command line args\n");
   printf("-s x        Used to change the number of children processes being made to the value x\n");
   printf("-l filename Used to change the name of the log file being written to\n");
   printf("-i y        Used to change the number of time a child increments and prints to the value y\n");
   printf("-t z        Used to change the time the master process will wait until killing all processes to the value z\n");
}
                                                              198,1         Bot
   }
   else if(s == SIGALRM)
   {
      fprintf(stderr, "Timer ran out. Killing all child processes\n");
   }
   for(i = 0; i < varx; i++)
   {
      kill(pids[i], SIGTERM);
   }

}
void  ChildProcess(int i)   //convert data to strings and pass to slave as command line args
{
   char slavevary[100];
   char slaveprocnum[100];
   char slavevarx[100];
   sprintf(slavevary, "%d", vary);
   sprintf(slaveprocnum, "%d", i);
   sprintf(slavevarx, "%d", varx);

   execlp("slave", "slave", slaveprocnum, slavevary, filename, slavevarx, (char *) 0);
}

void printstuff(void)  //print all help of command options
{
   printf("The command line arguments avialable are:\n");
   printf("-h          Used to display all command line args\n");
   printf("-s x        Used to change the number of children processes being made to the value x\n");
   printf("-l filename Used to change the name of the log file being written to\n");
   printf("-i y        Used to change the number of time a child increments and prints to the value y\n");
   printf("-t z        Used to change the time the master process will wait until killing all processes to the value z\n");
}

