#ifndef __HELPER_H__
#define __HELPER_H__
#include "tree.h"
#include "mylist.h"

void printTreeNode(void *treeNode, int depth);

void printTree(struct Tree *tree);

void printList(struct List *list);

void freeTreeData(struct Tree *tree);

struct Node *addTreeChild(struct Node *parent, int dir);

int **init2DArray(int rows, int cols);

#endif
