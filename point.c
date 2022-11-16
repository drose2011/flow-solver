#include "point.h"
#include <stdlib.h>


struct Point *newPoint(int row, int col)
{
	struct Point *pt = malloc(sizeof(struct Point));
	pt->col = col;
	pt->row = row;
	return pt;
}

struct Point *copyPoint(struct Point *og)
{
	struct Point *copy = malloc(sizeof(struct Point));
	copy->col = og->col;
	copy->row = og->row;
	return copy;
}
