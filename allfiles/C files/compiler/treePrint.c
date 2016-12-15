#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"
#include "compilertree.h"

void preorder ( struct treenode *sr )//sort tree using preorder traversals
{
	if ( sr != '\0' ) 
	{
		sr -> treetoken;
		preorder ( sr -> child1 );
		preorder ( sr -> child2 );
                preorder ( sr -> child3 );
                preorder ( sr -> child4 );
	}
}

void printpreordertree(node *sr)//print tree using preorder traversals
{
	if (sr == '\0') return;
 
	int i;
	for (i = 0; i < sr->level; i++)
	{
		printf("  ");
	}
	printf("%s: %s on line %d\n", sr->label, sr->treestring, sr->treelinenum);
	printpreordertree(sr->child1);
	printpreordertree(sr->child2);
        printpreordertree(sr->child3);
        printpreordertree(sr->child4);
}

