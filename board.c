#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"

extern int debug;

// Constructing 2d int array
int **make_board(char *input_board, int *numPath) 
{
	int i;
	
	char input_copy[strlen(input_board) + 1];
	strcpy(input_copy, input_board);
	char *tmp = strtok(input_copy," ");

	int rows = 0;
	int cols = 0;

	// count cols and rows
	while(tmp != NULL) {
		while(tmp[0] != '\n') {
			if(rows == 0) {
				cols++;
			}
			tmp = strtok(NULL, " ");
		}
		rows++;
		tmp = strtok(NULL, " ");
	}
	
	
	int **board = malloc((rows * sizeof(int *)) + 1);
	board[0] = malloc(2 * sizeof(int));
	board[0][0] = rows;
	board[0][1] = cols;
	for(i = 0; i < rows; i++) 
		board[i+1] = malloc(cols * sizeof(int));

	
	strcpy(input_copy, input_board);
	tmp = strtok(input_copy," ");
	int curRow = 1;
	int curCol = 0;
	int max = 0;
	while(tmp != NULL) {
		if(tmp[0] == '\n') {
			curRow++;
			curCol=0;
		} else {
			board[curRow][curCol] = atoi(tmp);
			if(board[curRow][curCol] > max)
				max = board[curRow][curCol];
			curCol++;
		}
		tmp = strtok(NULL, " ");
	}
	*numPath = max;
	return board;
}

char *colors(int num) 
{
	char *colors[8] = {"0", "1;31", "1;32", "1;33", "1;34", "1;35", "1;36", "1;37"};
	return colors[num % 8];
}

// Print board
void printBoard(int **board) 
{
	int rows = board[0][0];
	int cols = board[0][1];
	board++;
	for(int i=0;i < rows;i++) {
		for(int j =0;j < cols; j++) {
			printf("\033[%sm",colors(board[i][j]));
			printf("%2d ",board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	printf("\033[%sm",colors(0));
}

// free board
void free_board(int **board) 
{
	int rows = board[0][0];
	for(int i=0; i<rows; i++)
		free(board[i+1]);
	free(board);
}
