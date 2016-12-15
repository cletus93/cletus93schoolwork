#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"
#include "compilertree.h"


void insert (const char *passedlabel, struct treenode **sr, int levelcount, int linecount)
{
   *sr = (struct treenode*) malloc ( sizeof ( struct treenode ) );

   strcpy((*sr)->label, passedlabel);
   (*sr)->child1 = NULL;
   (*sr)->child2 = NULL;
   (*sr)->child3 = NULL;
   (*sr)->child4 = NULL;
   (*sr)->level = levelcount;
   (*sr)->treelinenum = linecount;
}
