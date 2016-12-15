#include <time.h>

typedef struct data_struct
{
   time_t time;
   char * string;
} data_t;

int addmsg(data_t data, const char *srcname, int errno);
void clearlog(void);
char *getlog(void);
int savelog(char *filename);
