#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "point.h"

// TODO:
// 	- chang to variable number of children

extern int debug;


void initTree(struct Tree *tree, void *data)
{
	tree->root = newChild(data,NULL);
}

int isLeaf(struct Node *node) {
	if(node->childN == NULL &&
	   node->childS == NULL &&
	   node->childW == NULL &&
	   node->childE == NULL) {
		return 1;
	}
	return 0;
}

struct Node *newChild(void *data, struct Node *parent)
{
	struct Node *new = malloc(sizeof(struct Node));
	new->data = data;
	new->parent = parent;
	new->childN = NULL;	
	new->childE = NULL;	
	new->childS = NULL;	
	new->childW = NULL;
	return new;
}

void traverseTree_helper(struct Node *node, void (*f)(void *, int), int depth)
{
	if(node == NULL) 
		return;

	traverseTree_helper(node->childN, f, depth + 1);
	traverseTree_helper(node->childE, f, depth + 1);
	(*f)(node->data, depth);
	traverseTree_helper(node->childS, f, depth + 1);
	traverseTree_helper(node->childW, f, depth + 1);
}
void traverseTree(struct Tree *tree, void (*f)(void *, int))
{
	traverseTree_helper(tree->root, f, 0);
}

void freeTree_helper(struct Node *node)
{
	if(node == NULL)
		return;

	freeTree_helper(node->childN);
	freeTree_helper(node->childE);
	freeTree_helper(node->childS);
	freeTree_helper(node->childW);
	free(node);
}
void freeTree(struct Tree *tree) 
{
	freeTree_helper(tree->root);
}
