//Randall Rootz, 9/10/16, this program was built to experiment with command line options using getopt and to display the us of perror and logfiles.
#include <stdio.h>
#include <getopt.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "log.h"

void printstuff(void);

int main (int argc, char **argv)
{
   int varx = 42, MAXSIZE = 0xFFF, fno, c, option_index = 0, errno = 0;
   char proclnk[0xFFF];
   char filename[] = "logfile.txt";
   char *file = filename;
   time_t ntime;
   ssize_t r;
   data_t logdata;

   FILE *f;

   ntime = time(NULL);
   logdata.time = ntime;

    if(argc < 2)
    {
       printf("You need to enter an option, try -h\n");
       exit(2);
    }
      while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"help",  no_argument,       0,  'h' },
        };

       c = getopt_long_only(argc, argv, "hn:l:", long_options, NULL);
        if (c == -1)
            break;

       switch (c) {
      case 'h':
        printstuff();
        break;
      case 'n':
        f = fopen (filename, "a+");
        if ( f == NULL)
        {
           perror("Can't open file\n");
           exit(-1);
        }
        varx = atoi(optarg);
        if(varx == 42)
        {
           perror("You chose the magic number!");
        }
        errno = 1;
        logdata.string = "";
        logdata.time = ntime;
        addmsg(logdata, argv[0], errno);
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

        if(filename == "logfile.txt")
        {
           perror("Oh, you changed your mind.");
        }
        errno = 2;
        logdata.string = "";
        logdata.time = ntime;
        addmsg(logdata, argv[0], errno);
        break;
      case '?':
         if (optopt == 'c')
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
         else if (isprint (optopt))
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);
         else
            fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
      default:
        printf("Error");
        exit(1);
      }
   }
   for (option_index = optind; option_index < argc; option_index++)
    printf ("Non-option argument %s\n", argv[option_index]);

   fprintf(f, "The value of varx is: %d\n\n", varx);

   savelog(file);
   clearlog();
   fclose(f);

   return 0;
}

void printstuff(void)
{
   printf("The command line arguments avialable are:\n");
   printf("-h          Used to display all command line args\n");
   printf("-help       Used to display all command line args\n");
   printf("-n x        Used to change the value of a variable in the program being run to the value x\n");
   printf("-l filename Used to change the name of the log file being written to\n");
}
