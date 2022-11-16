#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helper.h"
#include "board.h"

extern int debug;

// finds number of rows and cols in input. puts results into rows and cols
void getDimentions(int *rows, int *cols, char *input, char *delim)
{
	int row_count = 0;
	int col_count = 0;

	char input_copy[strlen(input) + 1];
	strcpy(input_copy, input);
	char *tmp = strtok(input_copy, delim);

	while(tmp != NULL) {
		while(tmp[0] != '\n') {
			if(row_count == 0)
				col_count++;
			tmp = strtok(NULL, " ");
		}
		row_count++;
		tmp = strtok(NULL, " ");
	}
	*rows = row_count;
	*cols = col_count;
}

// fills board with input, puts number of paths into numPath
void fillBoard(int **board, char *input, int *numPath, char *delim)
{
	int max = 0;
	char input_copy[strlen(input) + 1];
	strcpy(input_copy, input);
	char *tmp = strtok(input_copy, delim);

	int curRow = 1;
	int curCol = 0;
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
}

// Constructing 2d int array
int **makeBoard(char *input_board, int *numPath) 
{
	int rows, cols;

	char *delim = " ";

	getDimentions(&rows, &cols, input_board, delim);

	int **board = init2DArray(rows,cols);	
	
	fillBoard(board, input_board, numPath, delim);

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
void freeBoard(int **board) 
{
	int rows = board[0][0];
	for(int i=0; i<rows; i++)
		free(board[i+1]);
	free(board);
}
