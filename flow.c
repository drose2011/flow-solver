//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include "board.h"
#include "quadtree.h"
#include <stdio.h>
#include <stdlib.h>
#include "mylist.h"

int debug = 0;







int openDirections(int **board, int col, int row, int path, int rows, int cols)
{
	int out = 0;
	if(row > 0 && (board[row-1][col]==0 || board[row-1][col]==path))
		out += 1;
	if(row < rows-1 && (board[row+1][col]==0 || board[row+1][col]==path))
		out += 2;
	if(col > 0 && (board[row][col-1]==0 || board[row][col-1]==path))
		out += 4;
	if(col < cols-1 && (board[row][col+1]==0 || board[row][col+1]==path))
		out += 8;
	return out;		
}

int fillNode(int **board, struct Node *node, int path, int rows, int cols, int depth, int max_depth) {
	int row = node->row;
	int col = node->col;

	if(board[row][col] == path)
		return 0;
	if(depth > max_depth) 
		return 1;

	board[row][col] = -1;
	int open = openDirections(board,col,row,path, rows, cols);
	if(open == 0) {
		board[row][col]=0;
		return 1;
	}

	if(debug) {
		fprintf(stderr, "Filling node at location (row=%d,col=%d)\n", row, col);
		fprintf(stderr, "(open = %d)open spaces:\n\tOpen to North:%d\n\tOpen to South:%d\n\tOpen to West:%d\n\tOpen to East:%d\n", open, (open) & 1, (open >> 1) & 1, (open >> 2) & 1, (open >> 3) & 1);
	}

	// stop if we have reached the other root or a dead end (clean the dead end)

	int prune;
	struct Node *new;
	// continue to build tree to the
	// North
	if((open) & 1) {
		if(debug) {
			fprintf(stderr, "Open to North\n");
		}
		new = addChild(node,0);	
		prune = fillNode(board,new,path,rows,cols,depth+1,max_depth);
		if(prune) {
			free(new);
			node->childN=NULL;
		}
		// add path to the north
		//board[row-1][col] = -1*
	}
	// South
	if((open >> 1) & 1) {
		if(debug) {
			fprintf(stderr, "Open to South\n");
		}
		new = addChild(node,2);
		prune = fillNode(board,new,path,rows,cols,depth+1,max_depth);
		if(prune) {
			free(new);
			node->childS=NULL;
		}
		// add path to the east
	}
	// West
	if((open >> 2) & 1) {
		if(debug) {
			fprintf(stderr, "Open to West\n");
		}
		new = addChild(node,3);
		prune = fillNode(board,new,path,rows,cols,depth+1,max_depth);
		if(prune) {
			free(new);
			node->childW=NULL;
		}
		// add path to the south
	} 
	// East
	if((open >> 3) & 1) {
		if(debug) {
			fprintf(stderr, "Open to East\n");
		}
		new = addChild(node,1);
		prune = fillNode(board,new,path,rows,cols,depth+1,max_depth);
		if(prune) {
			free(new);
			node->childE=NULL;
		}
	}
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
	initTree(tree,col,row);

	if(debug) {
		fprintf(stderr, "initiated Tree for path %d\n", path);
	}

	board[row][col] = -1;
	fillNode(board, tree->root, path, rows, cols, 0, max_depth);
	board[row][col] = path;

	if(debug) {
		fprintf(stderr, "Tree filled for path %d\n", path);
	}

	board--;	
	return tree;
}

void printListNode(void *node)
{
	if(node == NULL)
		printf("NULL, ");
	else {
		struct Node *casted = (struct Node *)node;
		printf("(%d,%d), ", casted->row, casted->col);
	}
}
void printList(struct List *list)
{
	printf("(row,col)\n[");
	traverseList(list, &printListNode);
	printf("]\n");
}

void applyPathToBoard(int **board, struct List *overallPath, int numPath)
{
	board++;
	struct Node *cur;
	while(numPath > 0) {
		while((cur = popFront(overallPath)) != NULL) {
			board[cur->row][cur->col] = numPath;	
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
	struct Node *n1 = (struct Node *)listNode;
	struct Node *n2 = (struct Node *)curNode;
	if(n1->row == n2->row && n1->col == n2->col) {
		return 0;
	}
	return 1;
}
int validMove(int **board, struct List *visited, struct Node *curNode) {
	if(debug)
		fprintf(stderr, "In validMove\n");
	if(curNode == NULL)
		return 1;
	if(isLeaf(curNode)) {
		if(board[curNode->row+1][curNode->col] == 0) {
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


	if(validMove(board, visited, curNode->childN) == 0) {
		addFront(visited, curNode);
		return 0;
	}
	if(validMove(board, visited, curNode->childS) == 0) {
		addFront(visited, curNode);
		return 0;
	}
	if(validMove(board, visited, curNode->childW) == 0) {
		addFront(visited, curNode);
		return 0;
	}
	if(validMove(board, visited, curNode->childE) == 0) {
		addFront(visited, curNode);
		return 0;
	}
	return 1; // None of the nodes returned valid paths
}
int getValidPath(int **board, struct List *visited, struct Tree *paths, int numPaths) {
	if(numPaths == 0) { 
		if(debug)
			fprintf(stderr,"Finished last path, passing 0 back up.\n");
		return 0;
	}
	if(debug)
		fprintf(stderr,"Finding tree for numPaths = %d.\n", numPaths);
	struct Node *cur = (paths)->root;
	if(debug) fprintf(stderr, "Test, %d, %d\n", cur->row, cur->col);
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

#if 0
	char *input_board = "0\n";		// 1x1
	
	char *input_board = "00\n"		// 2x2
			    "00\n";

	char *input_baord = "000\n"		// 3x3
			    "000\n"
			    "000\n";

	char *input_board = "0000\n"		// 4x4
			    "0000\n"
			    "0000\n"
			    "0000\n";

	char *input_board = "00000\n"		// 5x5
			    "00000\n"
			    "00000\n"
			    "00000\n"
			    "00000\n";

	char *input_board = "000000\n"		// 6x6
			    "000000\n"
			    "000000\n"
			    "000000\n"
			    "000000\n"
			    "000000\n";

	char *input_board = "0000000\n"		// 7x7
			    "0000000\n"
			    "0000000\n"
			    "0000000\n"
			    "0000000\n"
			    "0000000\n"
			    "0000000\n";
	
	char *input_board = "00000000\n"	// 8x8
			    "00000000\n"
			    "00000000\n"
			    "00000000\n"
			    "00000000\n"
			    "00000000\n"
			    "00000000\n"
			    "00000000\n";

	char *input_board = "000000000\n"	// 9x9
			    "000000000\n"
			    "000000000\n"
			    "000000000\n"
			    "000000000\n"
			    "000000000\n"
			    "000000000\n"
			    "000000000\n"
			    "000000000\n";

	char *input_board = "0000000000\n"	// 10x10
			    "0000000000\n"
			    "0000000000\n"
			    "0000000000\n"
			    "0000000000\n"
			    "0000000000\n"
			    "0000000000\n"
			    "0000000000\n"
			    "0000000000\n"
			    "0000000000\n";
	
	char *input_board = "1001\n"
			    "2002\n"
			    "3003\n"
			    "4004\n";
	
	char *input_board = "1000001\n"
			    "2000002\n"
			    "3000003\n"
			    "4000004\n"
			    "5000005\n"
			    "6000006\n"
			    "7000007\n";
	
	char *input_board = "10001\n"
			    "00000\n"
			    "02304\n"
			    "00003\n"
			    "04002\n";
	
	char *input_board = "10001\n"		// 5x5
			    "00000\n"
			    "02304\n"
			    "00003\n"
			    "04002\n";
	
	char *input_board = "12002\n"
			    "00001\n";

	
	char *input_board = "1020230\n"		// 7x7
			    "4050060\n"
			    "0000500\n"
			    "0000603\n"
			    "1740000\n"
			    "0000070\n"
			    "0000000\n";
	
	char *input_board = "00001210\n"	// 8x8
			    "00003040\n"
			    "00500000\n"
			    "00000240\n"
			    "00560000\n"
			    "00000700\n"
			    "06730000\n"
			    "00000000\n";
	
	char *input_board = "10000002\n"	// 8x8
			    "00000030\n"
			    "00040050\n"
			    "00000003\n"
			    "00200500\n"
			    "00000610\n"
			    "06000000\n"
			    "00040000\n";

	char *input_board = "000000\n"		// 6x6
			    "120300\n"
			    "040000\n"
			    "050001\n"
			    "040032\n"
			    "500000\n";


#endif
	
	char *input_board = "10000002\n"	// 8x8
			    "00000030\n"
			    "00040050\n"
			    "00000003\n"
			    "00200500\n"
			    "00000610\n"
			    "06000000\n"
			    "00040000\n";
	
			    
	int numPaths;
	

	int **board = make_board(input_board, &numPaths);

	printBoard(board);

	int max_depth = max(board[0][0],board[0][1]) * 3;

	if(debug) {
		fprintf(stderr, "parsed board, printing:\n");
		printBoard(board);
	}

	struct Tree paths[numPaths];
	for(int i=0; i<numPaths; i++) {
		paths[i] = *buildPathTree(board, i+1, max_depth);
		printf("Finished tree for path %d\n", i+1);
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
		freeTree(&(paths[i]));
	}
	free_board(board);
}










