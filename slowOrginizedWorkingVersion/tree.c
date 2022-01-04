#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "point.h"

// TODO:
// 	- chang to variable number of children

extern int debug;

int numChildren;

void initTree(struct Tree *tree, void *data, int inputNumChildren)
{
	numChildren = inputNumChildren;
	tree->root = newChild(data,NULL);
}

int isLeaf(struct Node *node) {
	struct Node **childList = node->childList;
	for(int i = 0; i < node->numChildren; i++) {
		if(childList[i] != NULL)
			return 0;
	}
	return 1;
}

struct Node *newChild(void *data, struct Node *parent)
{
	struct Node *new = malloc(sizeof(struct Node));
	new->data = data;
	new->parent = parent;
	if(parent != NULL)
		new->numChildren = parent->numChildren;
	else
		new->numChildren = numChildren;
	new->childList = malloc(sizeof(struct Node *) * new->numChildren);
	struct Node **childList = new->childList;
	for(int i = 0; i < new->numChildren; i++) {
		childList[i] = NULL;
	}
	return new;
}

void traverseTree_helper(struct Node *node, void (*f)(void *, int), int depth)
{
	if(node == NULL) {
		return;
	}
	struct Node **childList = node->childList;
	for(int i = 0; i < node->numChildren; i++) {
		traverseTree_helper(childList[i], f, depth + 1);
	}
	(*f)(node->data, depth);
}
void traverseTree(struct Tree *tree, void (*f)(void *, int))
{
	traverseTree_helper(tree->root, f, 0);
}

void freeTree_helper(struct Node *node)
{
	if(node == NULL)
		return;
	
	struct Node **childList = node->childList;
	for(int i = 0; i < node->numChildren; i++) {
		freeTree_helper(childList[i]);
	}	
	free(node);
}
void freeTree(struct Tree *tree) 
{
	freeTree_helper(tree->root);
}
