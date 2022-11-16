#ifndef __POINT_H__
#define __POINT_H__

struct Point 
{
	int row;
	int col;
};

struct Point *newPoint(int row, int col);

struct Point *copyPoint(struct Point *);

#endif
