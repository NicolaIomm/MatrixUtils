CC = gcc -Wall
LDFLAGS = -lm -g

all: matrixDet

matrixDet: matrixDet.c utils.h
	$(CC) -o matrixDet matrixDet.c $(LDFLAGS)

.PHONY: clean

clean:
	rm -f matrixDet
