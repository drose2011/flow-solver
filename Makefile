#
# Makefile for lab 6, part 1
#

CC  = gcc

INCLUDES = 
CFLAGS   = -g -Wall $(INCLUDES)

LDFLAGS = -g 
LDLIBS = -L. -lmylist

flow: flow.o board.o quadtree.o

board.o: board.c board.h

flow.o: flow.c board.h quadtree.h

quadtree.o: quadtree.c quadtree.h

.PHONY: clean
clean:
	rm -f *.o *~ a.out core flow

.PHONY: all
all: clean flow

