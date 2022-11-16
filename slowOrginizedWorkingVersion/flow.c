//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include <time.h>
#include "board.h"
#include "tree.h"
#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include "mylist.h"
#include "point.h"

// TODO:
// 	- Move flow specific tree functions
// 	- free points before freeing all the nodes in the tree
// 	- Change to match tree void *data
// 	- Change to match variable number of children
// 	- split up valid move function into find first path and then find next path from pointer to a solution
// 	- have openDirections only check for one -1 around point so prev doesnt have to be passed


int debug = 0;



int openDirections(int **board, struct Point *pt, int path, struct Point *crnr)
{
	int out = 0;
	int adj_counter = 0;
	int row = pt->row;
	int col = pt->col;
	int rows = crnr->row;
	int cols = crnr->col;
	// North
	if(row > 0) {
		if(board[row-1][col] == -1)
			adj_counter += 1;
		if ((board[row-1][col]==0 || board[row-1][col]==path))
			out += 1;
	}
	// East
	if(col < cols-1) {
		if(board[row][col+1] == -1)
			adj_counter += 1;
		if ((board[row][col+1]==0 || board[row][col+1]==path))
			out += 2;
	}
	// South	
	if(row < rows-1) {
		if(board[row+1][col] == -1) 
			adj_counter += 1;
		if ((board[row+1][col]==0 || board[row+1][col]==path))
			out += 4;
	}
	// West
	if(col > 0) {
		if(board[row][col-1] == -1)
			adj_counter += 1;
		if ((board[row][col-1]==0 || board[row][col-1]==path))
			out += 8;
	}
	if(adj_counter > 1)
		return 0;	
	return out;		
}

int fillNode(int **board, struct Node *node, int path, struct Point *crnr, int depth, int max_depth) {
	// TODO: make fillNode just take the data
	struct Point *pt = node->data;
	int row = pt->row;
	int col = pt->col;

	if(board[row][col] == path)
		return 0;
	if(depth > max_depth) 
		return 1;

	// Set current location as visited
	board[row][col] = -1;
	int open = openDirections(board,pt,path,crnr);
	if(open == 0) {
		board[row][col]=0;
		return 1;
	}

	if(debug) {
		fprintf(stderr, "Filling node at location (row=%d,col=%d)\n", row, col);
		fprintf(stderr, "(open = %d)open spaces:\n\tOpen to North:%d\n\tOpen to East:%d\n\tOpen to South:%d\n\tOpen to West:%d\n", open, (open) & 1, (open >> 1) & 1, (open >> 2) & 1, (open >> 3) & 1);
	}

	// stop if we have reached the other root or a dead end (clean the dead end)

	int prune;
	struct Node *new;
	// Fill in each child if that direction is open
	for(int i = 0; i < node->numChildren; i++) {
		if((open >> i) & 1) {
			if(debug)
				fprintf(stderr, "Child #%d Open\n", i);
			new = addTreeChild(node,i);
			prune = fillNode(board,new,path,crnr,depth+1,max_depth);
			if(prune) {
				if(debug)
					fprintf(stderr, "No solution for this child!\n");
				free(new);
				node->childList[i]=NULL;
			}
		}
	}
	// Remove visited mark from location
	board[row][col]=0;
	if(isLeaf(node)) {
		return 1;
	}
	return 0;
}

struct Tree *buildPathTree(int **board, int path, int max_depth) 
{
	if(debug) {
		fprintf(stderr, "Starting to build tree\n");
	}
	int rows = board[0][0];
	int cols = board[0][1];
	struct Point *crnr = newPoint(rows,cols);
	int row;
	int col;
	board++;
	// find a root node
	for(row=0; row<rows; row++) {
		for(col=0; col<cols; col++) {
			if(board[row][col] == path)
				break;
		}
		if(board[row][col] == path)
			break;
	}
	if(debug) {
		fprintf(stderr, "Root for path %d found at: (row=%d,col=%d)\n", path, row, col);
	}
	
	struct Tree *tree = malloc(sizeof(struct Tree));
	struct Point *pt = newPoint(row,col);
	initTree(tree,pt,4);

	if(debug) {
		fprintf(stderr, "initiated Tree for path %d\n", path);
	}

	board[row][col] = -1;
	fillNode(board, tree->root, path, crnr, 0, max_depth);
	board[row][col] = path;

	if(debug) {
		fprintf(stderr, "Tree filled for path %d\n", path);
	}

	board--;	
	return tree;
}


void applyPathToBoard(int **board, struct List *overallPath, int numPath)
{
	board++;
	struct Node *cur;
	struct Point *pt;
	while(numPath > 0) {
		while((cur = popFront(overallPath)) != NULL) {
			pt = cur->data;
			board[pt->row][pt->col] = numPath;	
		}
		numPath--;
	}
}

int numSol;
int nodeComp(const void *listNode, const void *curNode) {
	if(listNode == NULL) {
		if(curNode == NULL) {
			if(debug)
				fprintf(stderr, "Looking for and found NULL\n");
			return 0;
		}
		return 1;
	}
	struct Point *pt1 = ((struct Node *)listNode)->data;
	struct Point *pt2 = ((struct Node *)curNode)->data;
	if(pt1->row == pt2->row && pt1->col == pt2->col) {
		return 0;
	}
	return 1;
}
int validMove(int **board, struct List *visited, struct Node *curNode) {
	if(debug)
		fprintf(stderr, "In validMove\n");
	if(curNode == NULL)
		return 1;
	struct Point *pt = curNode->data;

	if(isLeaf(curNode)) {
		if(board[pt->row+1][pt->col] == 0) {
			return 1;
		}
		if(numSol-- == 0) {
			if(debug)
				fprintf(stderr,"Solution Found! Passing back up to be added to visited.\n");
			addFront(visited, NULL);
			addFront(visited, curNode);
			return 0;
		}
		if(debug)
			fprintf(stderr,"Solution found, but looking for %d more solutions.\n", numSol);
		return 1;
	}
	if(debug)
		fprintf(stderr, "checking if the current node is in visited already\n");
	if(findListNode(visited, curNode, &nodeComp) != NULL) { 
		return 1;
	}

	for(int i = 0; i < curNode->numChildren; i++) {
		if(validMove(board, visited, curNode->childList[i]) == 0) {
			addFront(visited, curNode);
			return 0;
		}
	}
	return 1;

}
int getValidPath(int **board, struct List *visited, struct Tree **paths, int numPaths) {
	if(numPaths == 0) { 
		if(debug)
			fprintf(stderr,"Finished last path, passing 0 back up.\n");
		return 0;
	}
	if(debug)
		fprintf(stderr,"Finding tree for numPaths = %d.\n", numPaths);
	struct Node *cur = (paths[0])->root;
	numSol=0;
	if(validMove(board,visited,cur) == 1) {
		if(debug)
			fprintf(stderr,"No solution for current level, passing 1 back up.\n");
		return 1;
	}
	if(debug)
		fprintf(stderr,"Found a solution for the current level, continuing.\n");
	for(int i=1;getValidPath(board, visited, paths+1, numPaths-1) == 1;i++) {
		if(debug)
			fprintf(stderr,"Current path didn't work with one of the future trees, finding next possible answer.(i=%d)\n",i);
		while(popFront(visited) != NULL)
			;
		numSol=i;
		if(validMove(board,visited,cur) == 1)
			return 1;
	}
	return 0;
}

int max(int a, int b)
{
	if(a > b)
		return a;
	return b;
}


int main()
{
	char *input_boards[100] = {
		"0 \n ",			// 1x1, 0
		
		"0 0 \n "			// 2x2, 1
		"0 0 \n ",
		
		"0 0 0 \n "			// 3x3, 2
		"0 0 0 \n "
		"0 0 0 \n ",

		"0 0 0 0 \n "			// 4x4, 3
		"0 0 0 0 \n "
		"0 0 0 0 \n "
		"0 0 0 0 \n ",
		
		"0 0 0 0 0 \n "			// 5x5, 4
		"0 0 0 0 0 \n "
		"0 0 0 0 0 \n "
		"0 0 0 0 0 \n "
		"0 0 0 0 0 \n ",
		
		"0 0 0 0 0 0 \n "		// 6x6, 5
		"0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 \n ",
		
		"0 0 0 0 0 0 0 \n "		// 7x7, 6
		"0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 \n ",
		
		"0 0 0 0 0 0 0 0 \n "		// 8x8, 7
		"0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 \n ",
		
		"0 0 0 0 0 0 0 0 0 \n "		// 9x9, 8
		"0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 \n ",

		"0 0 0 0 0 0 0 0 0 0 \n "	// 10x10, 9
		"0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 \n ",
		
		"0 0 0 0 0 0 0 0 0 0 0 \n "	// 11x11, 10
		"0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 \n ",

		"0 0 0 0 0 0 0 0 0 0 0 0 \n "	// 12x12, 11
		"0 0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 0 \n ",
		
		"1 0 0 1 \n "			// 4x4, 12
		"2 0 0 2 \n "
		"3 0 0 3 \n "
		"4 0 0 4 \n ",
		
		"1 0 0 0 0 0 1 \n "		// 7x7, 13
		"2 0 0 0 0 0 2 \n "
		"3 0 0 0 0 0 3 \n "
		"4 0 0 0 0 0 4 \n "
		"5 0 0 0 0 0 5 \n "
		"6 0 0 0 0 0 6 \n "
		"7 0 0 0 0 0 7 \n ",
		
		"1 0 0 0 1 \n "			// 5x5, 14
		"0 0 0 0 0 \n "
		"0 2 3 0 4 \n "
		"0 0 0 0 3 \n "
		"0 4 0 0 2 \n ",
		
		"1 0 0 0 1 \n "			// 5x5, 15
		"0 0 0 0 0 \n "
		"0 2 3 0 4 \n "
		"0 0 0 0 3 \n "
		"0 4 0 0 2 \n ",
		
		"1 2 0 0 2 \n "			// 2x5, 16
		"0 0 0 0 1 \n ",
		
		"1 0 2 0 2 3 0 \n "		// 7x7, 17
		"4 0 5 0 0 6 0 \n "
		"0 0 0 0 5 0 0 \n "
		"0 0 0 0 6 0 3 \n "
		"1 7 4 0 0 0 0 \n "
		"0 0 0 0 0 7 0 \n "
		"0 0 0 0 0 0 0 \n ",
		
		"0 0 0 0 1 2 1 0 \n "		// 8x8, 18
		"0 0 0 0 3 0 4 0 \n "
		"0 0 5 0 0 0 0 0 \n "
		"0 0 0 0 0 2 4 0 \n "
		"0 0 5 6 0 0 0 0 \n "
		"0 0 0 0 0 7 0 0 \n "
		"0 6 7 3 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 \n ",
		
		"1 0 0 0 0 0 0 2 \n "		// 8x8, 19
		"0 0 0 0 0 0 3 0 \n "
		"0 0 0 4 0 0 5 0 \n "
		"0 0 0 0 0 0 0 3 \n "
		"0 0 2 0 0 5 0 0 \n "
		"0 0 0 0 0 6 1 0 \n "
		"0 6 0 0 0 0 0 0 \n "
		"0 0 0 4 0 0 0 0 \n ",
		
		"0 0 0 0 0 0 \n "		// 6x6, 20
		"1 2 0 3 0 0 \n "
		"0 4 0 0 0 0 \n "
		"0 5 0 0 0 1 \n "
		"0 4 0 0 3 2 \n "
		"5 0 0 0 0 0 \n ",
		
		"0 0 0 0 0 0 1 2 3 \n "		// 9x9, 21
		"0 4 0 0 0 0 0 0 0 \n "
		"5 0 0 1 0 0 0 0 0 \n "
		"0 0 0 2 0 0 0 0 0 \n "
		"0 3 0 0 0 0 0 6 0 \n "
		"0 0 0 0 7 0 0 0 0 \n "
		"0 0 0 0 0 0 8 0 8 \n "
		"0 0 0 5 0 0 7 6 9 \n "
		"4 0 0 0 0 9 0 0 0 \n ",

		"1 0 2 0 0 0 0 0 0 0 \n "	// 10x10, 22
		"3 0 0 0 4 0 5 6 0 0 \n "
		"0 0 0 0 7 0 0 0 0 0 \n "
		"0 1 2 0 0 0 0 0 0 0 \n "
		"0 0 8 0 0 7 0 0 0 0 \n "
		"4 3 0 0 0 0 0 5 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 9 10 0 0 \n "
		"0 9 0 0 0 6 0 0 0 0 \n "
		"0 0 0 0 0 10 8 0 0 0 \n ",
		
		"0 0 0 0 0 0 0 0 0 0 0 1 \n "	// 12x12, 23
		"0 0 0 0 0 0 0 2 0 0 0 3 \n "
		"0 0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 3 0 0 0 2 4 0 0 5 0 \n "
		"1 0 0 0 6 7 8 5 0 0 0 0 \n "
		"0 9 0 0 0 0 0 0 4 0 0 0 \n "
		"0 0 0 8 7 0 10 0 0 0 0 0 \n "
		"0 0 11 0 0 10 0 0 0 0 6 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 0 \n "
		"0 0 0 0 0 0 0 0 0 0 0 12 \n "
		"0 11 0 0 0 0 0 0 0 0 0 0 \n "
		"9 0 0 0 0 0 0 0 0 0 0 12 \n ",
		
		"0 0 1 0 0 0 0 0 0 0 2 \n "	// 11x11, 24
		"0 0 0 0 3 0 0 0 0 0 0 \n "
		"0 0 4 0 0 0 0 0 5 0 0 \n "
		"0 0 0 0 0 0 6 0 0 0 3 \n "
		"0 0 4 0 0 0 0 7 0 0 0 \n "
		"0 0 0 0 0 0 8 9 0 0 0 \n "
		"0 2 0 0 0 0 0 0 0 0 0 \n "
		"0 10 0 0 0 0 0 11 0 0 0 \n "
		"0 0 0 0 0 0 0 0 9 0 0 \n "
		"0 6 7 0 0 0 0 0 10 0 0 \n "
		"0 0 0 0 1 8 11 5 0 0 0 \n ",

	};
	
	for(int j=0; j<25; j++){

		clock_t begin = clock();

		char *input_board = input_boards[j];
		
		int numPaths;
		int **board = makeBoard(input_board, &numPaths);

		printBoard(board);

		int max_depth = max(board[0][0],board[0][1]) * 3.5;

		if(debug) {
			fprintf(stderr, "parsed board, printing:\n");
			printBoard(board);
		}

		struct Tree *paths[numPaths];
		for(int i=0; i<numPaths; i++) {
			paths[i] = buildPathTree(board, i+1, max_depth);
			printf("Finished tree for path %d\n", i+1);
			if(debug)
				printTree(paths[i]);
		}

		if(debug) {
			fprintf(stderr, "built tree for each path\n");
		}

		struct List overallPath;
		initList(&overallPath);

		if(debug)
			fprintf(stderr, "visited/final path initiated, calling valid path finder now.\n");

		if(getValidPath(board, &overallPath, paths, numPaths) == 1)
			fprintf(stderr, "No Solution\n");

		if(debug)
			fprintf(stderr, "valid path finder finished, cleaning up.\n");
		
		applyPathToBoard(board, &overallPath, numPaths);

		printBoard(board);

		removeAllListNodes(&overallPath);
		/*	
		for(int i=0; i<numPaths; i++) {
			printTree(&(paths[i]));
		}
		*/
		for(int i=0; i<numPaths; i++) {
			freeTree(paths[i]);
		}
		freeBoard(board);

		clock_t end = clock();
		printf("Time to execute: %f\n", (double)(end - begin) / CLOCKS_PER_SEC);
	}
}










