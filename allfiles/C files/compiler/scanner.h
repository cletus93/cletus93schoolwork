//Randall Rootz, 10/9/16, This is the header file to allow scannerproj.c to use the scannerparse function in scanner.c
#ifndef SCANNER_H
#define SCANNER_H
#include <stdio.h>
#include "token.h"

struct token TokenDriver(char *, int);

#endif
