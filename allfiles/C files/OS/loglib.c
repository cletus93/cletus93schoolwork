//Randall Rootz, 9/10/16, this program reads message input from logproj.c and then allocates memory and then passes the formatted message string to the logfile specified
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "log.h"

//construct data_t
typedef struct list_struct
{
   data_t item;
   struct list_struct *next;
} log_t;

//Pointers for top and bottom of que
static log_t *headptr = NULL;
static log_t *tailptr = NULL;
//adds a message to the data.string to be printed to logint addmsg(data_t data, const char *srcname, int errno)
{
   time_t seconds;
   log_t *tempnode = NULL;
   int nodesize;
   char timestring[] = ": Time: ";
   char errorstring[] = "Error: ";
   void *ptr = NULL;
   if (NULL == (ptr = malloc(sizeof(log_t))))
   {
      perror("malloc failed (sizeof(log_t))");
      return -1;
   }
   tempnode = (log_t *)ptr;
   tempnode->item.time = data.time;
   if(errno == 1)
   {
      data.string = "I am the worst error message, seg fault, FEAR ME!!!";
   }
   if(errno == 2)
   {
      data.string = "I just want to watch the world burn with error messages.";
   }
   char *timenano = ctime(&tempnode->item.time);   int messsize = strlen(": Time: 000000000 Error: ") + strlen(srcname) + strlen(timenano) + strlen(data.string) + 1;
   if (NULL == (ptr = malloc(messsize)))
   {
      perror("malloc failed (messsize)");
      free(tempnode);
      return -1;
   }
   tempnode->item.string = (char *)ptr;

   seconds = time(NULL);   sprintf(tempnode->item.string, "%s: Time: %ju000000000 Error: %s", srcname, seconds, data.string);
   tempnode->next = NULL;
   if(headptr == NULL)
   {
      headptr = tempnode;
   }
   else
   {
      tailptr->next = tempnode;
   }

   tailptr = tempnode;

   return 0;
}

//delete log contents
void clearlog(void)
{
   log_t *nextptr = headptr;
   while(nextptr != NULL)   {
      nextptr = headptr->next;
      free(headptr);
      headptr = nextptr;
   }
}

//builds full log message
char *getlog(void)
{
   size_t size = 0;  //individual message length
   size_t msglen;    //total length
   size_t offset = 0;//points to next message start
   log_t *node = headptr; //Start at beggining
   char *log = NULL; //Used to return the entire log

   while(node != NULL)
   {
      size += strlen(node->item.string);
      size++;
      node = node->next;
   }

   log = malloc(size);
   if(log == NULL)
   {
      perror("Malloc failed: ");
      return NULL;
   }

   node = headptr;

  while(node != NULL) {
        msglen = strlen(node->item.string);
        memcpy(log + offset, node->item.string, msglen);
        offset += msglen;
        memcpy(log + offset, "", 1);
        offset++;
        node = node->next;
    }
    memcpy(log + offset - 1, "\0", 1);
    return log;
}

//writes to file
int savelog(char *filename)
{
   FILE *f;
   char *logp;
   f = fopen(filename, "a+");
   if(!f)
   {
      perror("Error opening file: ");
      return -1;
   }

   logp = getlog();

   if(logp == NULL)
   {
      perror("Empty Log\n");
      return 0;
   }

   fprintf(f, "%s\n", logp);
   fclose(f);
   free(logp);
   return 0;
}
