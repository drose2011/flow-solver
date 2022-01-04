#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "tree.h"
#include "helper.h"

void printTreeNode(void *treeNode, int depth)
{
	for(int i=0; i < depth; i++) {
		printf("\t");
	}
	struct Point *pt = ((struct Node *)treeNode)->data;
	printf("*(row:%d,col:%d)\n", pt->row, pt->col);
}
void printTree(struct Tree *tree)
{
	traverseTree(tree, &printTreeNode);
}

void myFree(void *item, int x)
{
	free(item);
}
void freeTreeData(struct Tree *tree)
{
	traverseTree(tree, &myFree);
}

// dir 0=N, 1=E, 2=S, 3=W
struct Node *addTreeChild(struct Node *parent, int dir)
{
	struct Point *pt = copyPoint(parent->data);
	if(dir == 0)
		pt->row -= 1;
	else if(dir == 1)
		pt->col += 1;
	else if(dir == 2)
		pt->row += 1;
	else if(dir == 3)
		pt->col -= 1;
	else
		fprintf(stderr, "invalid direction fed to addChild\n");

	struct Node *new = newChild(pt, parent);
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

int **init2DArray(int rows, int cols)
{
	int **array = malloc((rows * sizeof(int *)) + 1);
	array[0] = malloc(2 * sizeof(int *));
	array[0][0] = rows;
	array[0][1] = cols;
	for(int i = 0; i < rows; i++)
		array[i+1] = malloc(cols * sizeof(int));
	return array;
}


