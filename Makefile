#
# Makefile for lab 6, part 1
#

CC  = gcc

INCLUDES =
CFLAGS   = -g -Wall $(INCLUDES)

LDFLAGS = -g 
LDLIBS = -L. -lmylist

flow: flow.o board.o tree.o point.o helper.o

board.o: board.c board.h

flow.o: flow.c board.h tree.h point.h

tree.o: tree.c tree.h point.h

point.o: point.c point.h

helper.o: helper.c helper.h

.PHONY: clean
clean:
	rm -f *.o *~ a.out core flow

.PHONY: all
all: clean flow

