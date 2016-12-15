#ifndef COMPILERTREE_H
#define COMPILERTREE_H
#include <stdio.h>
#include "token.h"

struct treenode//structure of binary search tree
{
	struct treenode *child1;
	struct treenode *child2;
        struct treenode *child3;
        struct treenode *child4;
        char label[20];
        char treetoken[20];
	char treestring[20];
	char treelinenum;
        char treetoken2[20];
        char treestring2[20];
        char treelinenum2;
        int level;
};
typedef struct treenode node;

#endif
