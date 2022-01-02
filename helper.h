#ifndef __HELPER_H__
#define __HELPER_H__
#include "tree.h"

void printTreeNode(void *treeNode, int depth);

void pringtTree(struct Tree *tree);

void freeTreeData(struct Tree *tree);

struct Node *addTreeChild(struct Node *parent, int dir);

int **init2DArray(int rows, int cols);

#endif
