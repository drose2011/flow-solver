#ifndef __TREE_H__
#define __TREE_H__

/*
 * Structs
 */
struct Node
{
	void *data;
	struct Node *parent;
	int numChildren;
	struct Node **childList;
};

struct Tree 
{
	struct Node *root;
};

/*
 * Functions
 */

void initTree(struct Tree *tree, void *data, int numChildren);

// returns 1 if each child is NULL, 0 otherwise
int isLeaf(struct Node *node);

// returns new child
struct Node *newChild(void *data, struct Node *parent);

// applies function to each data
void traverseTree(struct Tree *tree, void(*f)(void *, int));

// free's each node
void freeTree(struct Tree *tree);

#endif
