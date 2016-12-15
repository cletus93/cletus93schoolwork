//Randall Rootz, 9/29/2016, this program is a slave run by the children in a process spawning file named master.c, it prints data from shared memory to a file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

int sharedNum;     //global variables for shared memory
int sharedarray;
int shmkey = 122;
int arraykey = 33;
int *flag;
int *shmpointer;

volatile sig_atomic_t killSignal = 0;  //global varialbe for operations
FILE *outfile;

void critical_section(int, int, int);   //function prototypes
void sigHandler(int);

int main (int argc, char **argv)
{
   if(argc < 5)   //if there are not enough command line arg, quit out
   {
      printf("You need to enter all the correct data\n");
      printf("Please use the form,    execfile processnum writenum outfilename total_process_num\n");
      exit(0);
   }
   outfile = fopen (argv[3], "a+");   //open file to write to
   if ( outfile == 0 )
   {
      printf("Could not open file\n");
   }
   int k;      //set all variables to command line entries
   int procnum = atoi(argv[1]);
   int maxwrites = atoi(argv[2]);
   int totalproc = atoi(argv[4]);
   pid_t mypid = getpid();

   signal(SIGINT, SIG_IGN);     //ignore sig inturrupt
   signal(SIGQUIT, sigHandler); //do sighandler if you get a sigquit

   if ((sharedarray= shmget(arraykey, totalproc*sizeof(int), 0644 | IPC_CREAT)) == -1)   //allocate shared mem if one does not exist
   {
      perror("shmget failed");
      exit(1);
   }
   flag = (int *)shmat(sharedarray, 0, 0); //connect to shared mem flags


   enum state { idle, want_in, in_cs };  //make enumerated states for flags
   for(k = 0; k < totalproc; k++)
   {
      flag[k] = idle;  //initialize flags
   }
   int turn;
   turn = 0;
   int j;
      do
      {
         do
         {
            flag[procnum - 1] = want_in; // Raise my flag
            j = turn; // Set local variable
            // wait until its my turn
            while ( j != procnum - 1 )
            {
               j = ( flag[j] != idle ) ? turn : ( j + 1 ) % totalproc;
            }
            // Declare intention to enter critical section
            flag[procnum - 1] = in_cs;
            // Check that no one else is in critical section
            for ( j = 0; j < totalproc; j++ )
               if ( ( j != procnum - 1 ) && ( flag[j] == in_cs ) )
                  break;
         }
         while ( ( j < totalproc ) || ( turn != procnum - 1 && flag[turn] != idle ) );
         // Assign turn to self and enter critical section
         turn = procnum - 1;
         printf("Process %d let in critical section\n", procnum);
         critical_section(k, procnum, maxwrites);
         printf("Process %d finished critical section\n", procnum);
         // Exit section
         j = (turn + 1) % totalproc;
         while (flag[j] == idle)
         {
            j = (j + 1) % totalproc;
         }
         // Assign turn to next waiting process; change own flag to idle
         turn = j;
         flag[procnum - 1] = idle;
         break;
      }
      while ( 1 );
      kill(mypid, SIGTERM);   //send term to parent
      kill(mypid, SIGKILL);   //send kill to parent
   fclose(outfile);   //close output file
   return 0;
}

void critical_section(int k, int procnum, int maxwrites)
{

   time_t current_time;   //make time in format
   struct tm * time_info;
   char timeString[9];  // space for "HH:MM:SS\0"

   time(&current_time); //currnet time
   time_info = localtime(&current_time);

   strftime(timeString, sizeof(timeString), "%H:%M", time_info);

   time_t t;
   srand((unsigned) time(&t));

   if ((sharedNum= shmget(shmkey, 10, 0644 | IPC_CREAT)) == -1)//allocate mem for shared number
   {
      perror("shmget failed");
      exit(1);
   }

   shmpointer = (int*) shmat(sharedNum, 0, 0);   //attach to shared number
   if(procnum == 1)
   {
      *shmpointer = 0;
   }
   else
   {
      int newvalue = maxwrites * (procnum - 1);
      *shmpointer = newvalue;
   }

   for(k = 0; k < maxwrites; k++)   //loop through the number of writes
   {
      if(killSignal == 1)
      {
         printf("Slave %d received quit signal.\n", procnum);
         break;
      }
      sleep(k * (rand() % 2));
      *shmpointer += 1;
      printf("File modified by process number %d at time %s with sharedNum %d\n", procnum, timeString, *shmpointer);
      fprintf(outfile, "File modified by process number %d at time %s with sharedNum %d\n", procnum, timeString, *shmpointer);
   }
      sleep((k * (rand() % 2)) * 2);
   if (shmdt(shmpointer) == -1)   //dettach from shared mem
   {
      perror("shmdt for shared number");
      exit(1);
   }
   if (shmdt(flag) == -1)
   {
      perror("shmdt for shared flag");
      exit(1);
   }
   shmctl(sharedNum, IPC_RMID, 0);   //deallocate shared mem
   shmctl(sharedarray, IPC_RMID, NULL);

}

void sigHandler(int s) //set killnum if signal found
{
  killSignal = 1;
}
