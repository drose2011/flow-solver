#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"

extern int debug;

// Constructing 2d int array
int **make_board(char *input_board, int *numPath) 
{
	int i;
	
	int rows = 0;
	int cols = 0;
	for(i = 0; i < strlen(input_board); i++) {
		if(input_board[i] == '\n')
			rows++;		
		else if(rows == 0)
			cols++;
	}
	int **board = malloc((rows * sizeof(int **)) + 1);
	board[0] = malloc(2 * sizeof(int));
	board[0][0] = rows;
	board[0][1] = cols;
	int max=0;
	for(i = 0; i < rows; i++) {
		int *row = malloc(cols * sizeof(int *));
		board[i+1] = row;
		char tmp[2];
		tmp[1] = '\0';
		for(int j = 0; j < cols; j++) {
			tmp[0] = input_board[((cols+1) * (i)) + j];
			row[j] = atoi(tmp);
			if(row[j] > max)
				max = row[j];
		}
	}
	*numPath = max;
	return board;
}

// Print board
void printBoard(int **board) 
{
	int rows = board[0][0];
	int cols = board[0][1];
	board++;
	for(int i=0;i < rows;i++) {
		for(int j =0;j < cols; j++) {
			printf("%d",board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

// free board
void free_board(int **board) 
{
	int rows = board[0][0];
	for(int i=0; i<rows; i++)
		free(board[i+1]);
	free(board);
}
