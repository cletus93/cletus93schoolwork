//Randall Rootz, 9/17/16, This program creates a typedef for a binary tree thatkeeps track of how many duplicates of a node occur and thier level.

#ifndef TREE_H
#define TREE_H
#include <stdio.h>

struct treenode//structure of binary search tree
{
	struct treenode *left;
	int data;//tree data
	int datacount;//duplication count
	int level;//depth
	struct treenode *right;
};
typedef struct treenode node;

#endif