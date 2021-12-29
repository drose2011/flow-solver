#ifndef __QUADTREE_H__
#define __QUADTREE_H__

struct Node
{
	int col;
	int row;
	struct Node *childN;
	struct Node *childE;
	struct Node *childS;
	struct Node *childW;
};

struct Tree 
{
	struct Node *root;
};

void initTree(struct Tree *tree, int col, int row);

// dir 0=N, 1=E, 2=S, 3=W
struct Node *addChild(struct Node *parent, int dir);

int isLeaf(struct Node *node);

void traverseTree(struct Tree *tree, void (*f)(struct Node *, int), char middle);

void printTree(struct Tree *tree);

void freeTree(struct Tree *tree);

#endif
