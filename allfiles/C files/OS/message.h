#ifndef MESSAGE_H
#define MESSAGE_H
#include <stdio.h>

struct msgbuf {
        long mtype;
        char mtext[80];
       };

#endif
