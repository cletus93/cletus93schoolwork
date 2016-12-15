//Randall Rootz, 4/11/16,this program compares the different hieghts of trees based on different orders and size.
#include <iostream>
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
		void deletenode(int, node *&);
		void dodelete(node *&);
		void inorder(node *) const;
		void preorder(node *) const;
		void postorder(node *) const;
		int height(node *) const;


	public:
		BinarySearchTree()
		{
			root = NULL;
		}

		void insertnode(int);
		bool searchnode(int);
		void remove(int);

		int height() const
		{
			height(root);
		}

		void inorder() const
		{
			inorder(root);
		}

		void preorder() const
		{
			preorder(root);
		}

		void postorder() const
		{
			postorder(root);
		}
};

int main()
{
	BinarySearchTree tree;

	int arr[10] = {30, 10, 45, 38, 20, 50, 25, 33, 8, 12};

	for(int i = 0; i < 10; i++)
	{
		tree.insertnode(arr[i]);
	}

	string input;

	cout << "Tree was made" << endl;

	while(input != "6")
	{
		cout << endl;
		cout << "Type the number of the operation to perform:\n";
		cout << "1. Print Inorder Traversal with height\n";
		cout << "2. Print Postorder Traversal with height\n";
		cout << "3. Print Preorder Traversal with height\n";
		cout << "4. Search for numbers 38 and 9\n";
		cout << "5. Delete the number 10 and display all Traversals\n";
		cout << "6. Quit\n";
		cout << "Input: ";
		cin >> input;
		cout << endl;

		if(input == "1")
		{
			cout << "Inorder traversal is: ";
			tree.inorder();
			cout << endl;
			cout << "Height of the tree is: " << tree.height() << endl;
		}
		else if(input == "2")
		{
			cout << "Postorder traversal is: ";
			tree.postorder();
			cout << endl;
			cout << "Height of the tree is: " << tree.height() << endl;
		}
		else if(input == "3")
		{
			cout << "Preorder traversal is: ";
			tree.preorder();
			cout << endl;
			cout << "Height of the tree is: " << tree.height() << endl;
		}
		else if(input == "4")
		{
			tree.searchnode(38);
			tree.searchnode(9);
		}
		else if(input == "5")
		{
			tree.remove(10);
			cout << "Value 10 was deleted\n";
			cout << "Inorder traversal is: ";
			tree.inorder();
			cout << endl;
			cout << "Postorder traversal is: ";
			tree.postorder();
			cout << endl;
			cout << "Preorder traversal is: ";
			tree.preorder();
			cout << endl;
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

bool BinarySearchTree::searchnode(int key)
{
	node *nodePtr = root;
	cout << "Searching for " << key << endl;
	while(nodePtr)
	{
		if(nodePtr->data == key)
		{
			cout << key << " is in this node\n";
			return true;
		}
		else if(key < nodePtr->data)
		{
			cout << key << " < " << nodePtr->data << " going left\n";
			nodePtr = nodePtr->left;
		}
		else
		{
			cout << key << " > " << nodePtr->data << " going right\n";
			nodePtr = nodePtr->right;
		}
	}
	cout << key <<" is not in tree\n";
	return false;
}

void BinarySearchTree::remove(int key)
{
	deletenode(key, root);
}

void BinarySearchTree::deletenode(int key, node *&nodePtr)
{
	if(key < nodePtr->data)
	{
		deletenode(key, nodePtr->left);
	}
	else if(key > nodePtr->data)
	{	
		deletenode(key, nodePtr->right);
	}
	else
	{
		dodelete(nodePtr);
	}
}

void BinarySearchTree::inorder(node *nodePtr) const
{
	if(nodePtr)
	{
		inorder(nodePtr->left);
		cout << nodePtr->data << " ";
		inorder(nodePtr->right);

	}
}

void BinarySearchTree::postorder(node *nodePtr) const
{
	if(nodePtr)
	{
		postorder(nodePtr->left);
		postorder(nodePtr->right);
		cout << nodePtr->data << " ";
	}
}

void BinarySearchTree::preorder(node *nodePtr) const
{
	if(nodePtr)
	{
		cout << nodePtr->data << " ";
		preorder(nodePtr->left);
		preorder(nodePtr->right);
	}
}

void BinarySearchTree::dodelete(node *&nodePtr)
{
	node *tempPtr;

	if(nodePtr == NULL)
	{
		cout << "Empty Node" << endl;
	}
	else if(nodePtr->right == NULL)
	{
		tempPtr = nodePtr;
		nodePtr = nodePtr->left;
		delete tempPtr;
	}
	else if(nodePtr->left == NULL)
	{
		tempPtr = nodePtr;
		nodePtr = nodePtr->right;
		delete tempPtr;
	}	
	else
	{
		tempPtr = nodePtr->right;
		while(tempPtr->left)
		{
			tempPtr = tempPtr->left;
		}
		tempPtr->left = nodePtr->left;
		tempPtr = nodePtr;
		nodePtr = nodePtr->right;
		delete tempPtr;
	}
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

