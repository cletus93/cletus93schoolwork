//Author
//
//Date
//
//Log
//
//Randall Rootz, 9/29/2016, is a slave run by the children in a process spawning file named master.c, it prints data from shared memory to a file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main (int argc, char **argv)
{
   FILE *outfile;

   if(argc < 4)
   {
      printf("You need to enter all the correct data\n");
      printf("Please use the form,    execfile processnum writenum outfilename\n");
      exit(0);
   }

   int i;
   int procnum = atoi(argv[1]);
   int maxwrites = atoi(argv[2]);
   int yy = 1;
   outfile = fopen ( argv[3], "a" );

   if ( outfile == 0 )
   {
      printf("Could not open file\n");
   }

   time_t current_time;
   struct tm * time_info;
   char timeString[9];  // space for "HH:MM:SS\0"

   time(&current_time);
   time_info = localtime(&current_time);

   strftime(timeString, sizeof(timeString), "%H:%M", time_info);

   time_t t;
   srand((unsigned) time(&t));

   for(i = 0; i < maxwrites; i++)
   {
      sleep(i * (rand() % 2));
      printf("File modified by process number %d at time %s with sharedNum %d\n", procnum, timeString, yy);
      fprintf(outfile, "File modified by process number %d at time %s with sharedNum %d\n", procnum, timeString, yy);
      sleep(2 * i * (rand() % 2));
   }
   return 0;
}
