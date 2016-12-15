//This program is designed to be a template of how to build a tree.
#ifndef TREE_H
#define TREE_H
#include <stdio.h>

struct treenode
{
	struct treenode *left;
	int data;
	int datacount;
	int level;
	struct treenode *right;
};
typedef struct treenode node;

void insert ( int, struct treenode **, int);
void inorder ( struct treenode * );
void preorder ( struct treenode * );
void postorder ( struct treenode * );
void printinordertree (node *sr);
void printpreordertree (node *sr);
void printpostordertree (node *sr);
void fprintinordertree (node *sr, FILE *const outfile);
void fprintpreordertree (node *sr, FILE *const outfile);
void fprintpostordertree (node *sr, FILE *const outfile);


void insert ( int num, struct treenode **sr, int levelcount)
{
	if ( *sr == '\0' )
	{
		*sr = (struct treenode*) malloc ( sizeof ( struct treenode ) );

		( *sr ) -> left = 0;
		( *sr ) -> data = num;
		( *sr ) -> datacount = 1;
		( *sr ) -> level = levelcount;
		( *sr ) -> right = 0;
	}
	if (num == (*sr)->data)
    {
       ((*sr)->datacount)++;
    }
	if(num < (*sr)-> data)
	{
		levelcount++;
		insert ( num, &( *sr )-> left, levelcount );
	}
	else if(num > (*sr)-> data)
	{
		levelcount++;
		insert ( num, &( *sr )-> right, levelcount );
	}
}

void inorder ( struct treenode *sr )
{
	if ( sr != '\0' ) 
	{
		inorder ( sr -> left );
		sr -> data;
		inorder ( sr -> right );
	}
}

void preorder ( struct treenode *sr )
{
	if ( sr != '\0' ) 
	{
		sr -> data;
		preorder ( sr -> left );
		preorder ( sr -> right );
	}
}

void postorder ( struct treenode *sr )
{
	if ( sr != '\0' ) 
	{
		postorder ( sr -> left );
		postorder ( sr -> right );
		sr -> data;
	}
}

void printinordertree(node *sr)
{
	if (sr == '\0') return;
	
	printinordertree(sr->left);
	printf("%d:%d\t", sr->data, sr->datacount);
	printinordertree(sr->right);
}

void printpreordertree(node *sr)
{
	if (sr == '\0') return;

	printf("%d:%d\t", sr->data, sr->datacount);
	printpreordertree(sr->left);
	printpreordertree(sr->right);
}

void printpostordertree(node *sr)
{
	if (sr == '\0') return;

	printpostordertree(sr->left);
	printpostordertree(sr->right);
	printf("%d:%d\t", sr->data, sr->datacount);
}

void fprintinordertree(node *sr, FILE *const outfile)
{
	if (sr == '\0') return;
 
	fprintinordertree(sr->left, outfile);
	for (int i = 0; i < sr->level; i++)
	{
		fprintf(outfile, " ");
	}
	fprintf(outfile, "%d:%d\n", sr->data, sr->datacount);
	fprintinordertree(sr->right, outfile);
}

void fprintpreordertree(node *sr, FILE *const outfile)
{
	if (sr == '\0') return;
 
	for (int i = 0; i < sr->level; i++)
	{
		fprintf(outfile, " ");
	}
	fprintf(outfile, "%d:%d\n", sr->data, sr->datacount);
	fprintpreordertree(sr->left, outfile);
	fprintpreordertree(sr->right, outfile);
}

void fprintpostordertree(node *sr, FILE *const outfile)
{
	if (sr == '\0') return;
 
	fprintpostordertree(sr->left, outfile);
	fprintpostordertree(sr->right, outfile);
	for (int i = 0; i < sr->level; i++)
	{
		fprintf(outfile, " ");
	}
	fprintf(outfile, "%d:%d\n", sr->data, sr->datacount);
}
#endif
