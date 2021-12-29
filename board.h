#ifndef __BOARD_H__
#define __BOARD_H__

int **make_board(char *input_board, int *numPaths);
void printBoard(int **board);
void free_board(int **board);

#endif
