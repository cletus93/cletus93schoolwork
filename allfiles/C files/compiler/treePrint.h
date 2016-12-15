#ifndef TREEPRINT_H
#define TREEPRINT_H
#include <stdio.h>
#include "token.h"
#include "compilertree.h"

void preorder (struct treenode*);
void printpreordertree(struct treenode*);

#endif
