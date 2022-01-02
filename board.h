#ifndef __BOARD_H__
#define __BOARD_H__

int **makeBoard(char *input_board, int *numPaths);
void printBoard(int **board);
void freeBoard(int **board);

#endif
