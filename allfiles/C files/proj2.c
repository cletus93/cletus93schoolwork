//This program takes of list of integers from the user and sorts them using a tree from least to greatest.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"

int main ()
{
	FILE *outfile;

	outfile = fopen ("sortedlist.h", "wb");
	
	int i, n, count = 0;
	int arr[1000];

	printf("Please enter you list of integers, then type go: ");	
	
	while(scanf("%d", &n) == 1)
	{
	arr[count] = n;
	count++;
	}

	struct treenode *bt;
	bt = NULL;

	for ( i = 0; i < count; i++)
		insert ( arr[i], &bt, 0);

	inorder ( bt );

	printinordertree(bt);
	printf ("\n");
	fprintinordertree(bt, outfile);
	
	fclose(outfile);
	return 0;
}
