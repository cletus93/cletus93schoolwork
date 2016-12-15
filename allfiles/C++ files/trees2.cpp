//Randall Rootz, 4/11/16,this program compares the different hieghts of trees based the number of random integers added to the tree.
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

class BinarySearchTree
{
private:
	struct node
	{
		int data;
		node *left;
		node *right;
	};
			
	node *root;
														
	void insert(node *&, node *&);
	int height(node *) const;
public:
	BinarySearchTree()
	{
		root = NULL;
	}
						
	void insertnode(int);
	int height() const
	{
		height(root);
	}
};

int main()
{
	BinarySearchTree tree;

	int t[3] = {5, 10, 15};
	int N[3] = {100, 500, 1000};
	int totalheight;
	double avgheight;
	int n;
	int num;

	srand((unsigned int) time(NULL));

	for(int i=0; i < 3; i++)
	{
		for(int j=0; j < 3; j++)
		{
			totalheight = 0;
			for(int m=0; m < t[j]; m++)
			{
				n = N[i];
				int arr[n];
				for(int h=0; h < N[i]; h++)
				{
					num = rand() % 5000 + 1;
					arr[h] = num;
				}
				for(int y=0; y < N[i]; y++)
				{
					tree.insertnode(arr[y]);
				}
				totalheight += tree.height();
			}
			avgheight = totalheight / t[j];
			if(j == 2 && i == 0)
			{
				cout << "t = " << t[j] << endl;
				cout << "N = " << N[i] << endl;
				cout << "The average height is: " << avgheight << endl;
			}
		}
	}
	return 0;
}

void BinarySearchTree::insert(node *&nodePtr, node *&newNode)
{
	if (nodePtr == NULL)
	{
		nodePtr = newNode;
	}
	else if(newNode->data < nodePtr->data)
	{
		insert(nodePtr->left, newNode);
	}
	else
	{
		insert(nodePtr->right, newNode);
	}
}

void BinarySearchTree::insertnode(int key)
{
	node *newNode;
	newNode = new node;
	newNode->data = key;
	newNode->left = newNode->right = NULL;
	insert(root, newNode);
}

int BinarySearchTree::height(node *nodePtr) const
{
	if(nodePtr == NULL)
	{
		return -1;
	}
	else
	{
		int left = height(nodePtr->left);
		int right = height(nodePtr->right);
		return 1 + max(left,right);
	}
}

