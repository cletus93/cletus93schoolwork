//Randall Rootz, 10/9/16, This is the header file to allow scannerproj.c to use the scannerparse function in scanner.c
#ifndef TESTSCANNER_H
#define TESTSCANNER_H
#include <stdio.h>
#include "token.h"
#include "scanner.h"
#include "compilertree.h"
#include "treePrint.h"
#include "buildTree2.h"

char *scannerparse( int, FILE *, int, FILE *);//function to parse a file by white space

struct treenode* buildprogramtree(int);
struct treenode* program(int);
struct treenode* block(int);
struct treenode* vars(int);
struct treenode* mvars(int);
struct treenode* expr(int);
struct treenode* M(int);
struct treenode* T(int);
struct treenode* F(int);
struct treenode* R(int);
struct treenode* stats(int);
struct treenode* mStat(int);
struct treenode* stat(int);
struct treenode* in(int);
struct treenode* out(int);
struct treenode* iff(int);
struct treenode* loop(int);
struct treenode* assign(int);
struct treenode* RO(int);

#endif
