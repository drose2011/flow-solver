#include <stdio.h>
#include <stdlib.h>
#include "quadtree.h"

extern int debug;

struct Node *newChild(int col, int row)
{
	struct Node *new = malloc(sizeof(struct Node));
	new->col = col;
	new->row = row;
	new->childN = NULL;	
	new->childE = NULL;	
	new->childS = NULL;	
	new->childW = NULL;
	return new;
}

void initTree(struct Tree *tree, int col, int row)
{
	tree->root = newChild(col,row);
}

// dir 0=N, 1=E, 2=S, 3=W
struct Node *addChild(struct Node *parent, int dir)
{
	int newCol=parent->col;
	int newRow=parent->row;
	if(dir == 0)
		newRow -= 1;
	else if(dir == 1)
		newCol += 1;
	else if(dir == 2)
		newRow += 1;
	else if(dir == 3)
		newCol -= 1;
	else
		fprintf(stderr, "invalid direction fed to addChild\n");

	struct Node *new = newChild(newCol, newRow);
	//(parent->childN)+(sizeof(struct Node *) * dir)dd
	

	if(dir == 0)
		parent->childN = new;
	else if(dir == 1)
		parent->childE = new;
	else if(dir == 2)
		parent->childS = new;
	else if(dir == 3)
		parent->childW = new;
	else
		fprintf(stderr, "invalid direction fed to addChild\n");

	return new;
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
#if 0
void pruneTree_helper(int **board, struct Node *node, int path) {
	if(debug)
		fprintf(stderr, "In prune helper");
	if(isLeaf(node)) {
		return;
	}
	if(node->childN!=NULL && isLeaf(node->childN) && board[node->childN->row][node->childN->col] != path) {
		free(node->childN);
		node->childN = NULL;
	} else {pruneTree_helper(board,node->childN,path);}
	if(node->childS!=NULL && isLeaf(node->childS) && board[node->childS->row][node->childS->col] != path) {
		free(node->childS);
		node->childS = NULL;
	} else {pruneTree_helper(board,node->childS,path);}
	if(node->childW!=NULL && isLeaf(node->childW) && board[node->childW->row][node->childW->col] != path) {
		free(node->childW);
		node->childW = NULL;
	} else {pruneTree_helper(board,node->childW,path);}
	if(node->childE!=NULL && isLeaf(node->childE) && board[node->childE->row][node->childE->col] != path) {
		free(node->childE);
		node->childE = NULL;
	} else {pruneTree_helper(board,node->childE,path);}
}
void pruneTree(int **board, struct Tree *tree) {
	if(debug)
		fprintf(stderr, "in prune Tree\n");
	pruneTree_helper(board, tree->root, board[tree->root->row][tree->root->col]);
}
#endif
void traverseTree_helper(struct Node *node, void (*f)(struct Node *, int), char middle, int depth)
{
	if(node == NULL) 
		return;

	traverseTree_helper(node->childN, f, middle, depth + 1);
	traverseTree_helper(node->childE, f, middle, depth + 1);
	if(middle)
		(*f)(node, depth);
	traverseTree_helper(node->childS, f, middle, depth + 1);
	traverseTree_helper(node->childW, f, middle, depth + 1);
	if(!middle)
		(*f)(node, depth);
}
void traverseTree(struct Tree *tree, void (*f)(struct Node *, int), char middle)
{
	traverseTree_helper(tree->root, f, middle, 0);
}


void printNode(struct Node *node, int depth)
{
	for(int i=0; i < depth; i++) {
		printf("\t");
	}
	printf("*(row:%d,col:%d)\n", node->row, node->col);
}
void printTree(struct Tree *tree)
{
	traverseTree(tree, &printNode, 1);
}


void my_free(struct Node *node, int depth)
{
	free(node);
}
void freeTree(struct Tree *tree) 
{
	traverseTree(tree, &my_free, 0);
}

