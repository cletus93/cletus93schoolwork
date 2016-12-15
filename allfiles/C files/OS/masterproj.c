//Author
//
//Date
//
//Log
//
//Randall Rootz, 9/29/2016, this file was made to spawn child processes and calla slave process through the children, it also implements getopt
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

void printstuff(void);
void ChildProcess(int);
void ParentProcess(int);

int varx = 5, vary = 3, varz = 20;
char filename[] = "test.out";
pid_t pids[20];

int main (int argc, char **argv)
{
   int varx = 5, vary = 3, varz = 20, MAXSIZE = 0xFFF, fno, c, option_index = 0, n;
   char proclnk[0xFFF];
   char *file = filename;
   ssize_t r;

   FILE *f;

   if(argc < 2)
   {
      printf("You need to enter an option, try -h\n");
      exit(2);
   }

   while (1)
   {
      int this_option_optind = optind ? optind : 1;
      int option_index = 0;

   c = getopt (argc, argv, "hs:l:i:t:");
   if (c == -1)
      break;   switch (c)
   {
      case 'h':
        printstuff();
        break;
      case 's':
        f = fopen (filename, "a+");
        if ( f == NULL)
        {
           perror("Can't open file\n");
           exit(-1);
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
      case 'i':
        f = fopen (filename, "a+");
        if ( f == NULL)
        {
           perror("Can't open file\n");
           exit(-1);
        }
        vary = atoi(optarg);
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

   int i, status;
   n = varx;
   pid_t pid;

   for (i = 1; i <= varx; i++)
   {
      pids[i] = fork();
      if (pids[i] == 0)
      {
         sleep(i * 5);
         printf("I am process %d\n", i);
         ChildProcess(i);
         exit(0);
      }
      else
      {
         signal(SIGALRM, ParentProcess);
         alarm(varz);
      }
   }
}
void ParentProcess(int s)
{
   int i;
   printf("You reached the timeout\n");
   for(i = 0; i < varx; i++)
   {
      kill(pids[i], SIGTERM);
   }
}
void  ChildProcess(int i)
{
   char slavevary[100];
   char slaveprocnum[100];
   sprintf(slavevary, "%d", vary);
   sprintf(slaveprocnum, "%d", i);
   execlp("slave", "slave", slaveprocnum, slavevary, filename, (char *) 0);
}

void printstuff(void)
{
   printf("The command line arguments avialable are:\n");
   printf("-h          Used to display all command line args\n");   printf("-s x        Used to change the number of children processes being made to the value x\n");   printf("-l filename Used to change the name of the log file being written to\n");   printf("-i y        Used to change the number of time a child increments and prints to the value y\n");   printf("-t z        Used to change the time the master process will wait until killing all processes to the value z\n");
}
