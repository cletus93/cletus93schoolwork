//Randall Rootz, 9/17/16, This program inserts all integers passed to it into a binary search tree.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "node.h"
#include "traversals.h"

void insert ( int num, struct treenode **sr, int levelcount)//function to put a node into a tree, with duplications and depth 
{
	if ( *sr == '\0' )
	{
		*sr = (struct treenode*) malloc ( sizeof ( struct treenode ) );

		( *sr ) -> left = 0;
		( *sr ) -> data = num;
		( *sr ) -> datacount = 1;//duplication count
		( *sr ) -> level = levelcount;//depth
		( *sr ) -> right = 0;
	}
	if (num == (*sr)->data)
    {
       ((*sr)->datacount)++;//increment duplication count
    }
	if(num < (*sr)-> data)
	{
		levelcount++;//increase depth
		insert ( num, &( *sr )-> left, levelcount );//move to left sub tree

	}
	else if(num > (*sr)-> data)
	{
		levelcount++;//increase depth

		insert ( num, &( *sr )-> right, levelcount );//move to right sub tree
	}
}
