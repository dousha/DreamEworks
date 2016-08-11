CC=gcc
FLAGS=-O2

SRC=edhd.c
DEP=
OBJ=$(SRC:.c=.o)
TARGET=edhd

default:
	make all

all: $(OBJ) $(DEP)
	$(CC) $(OBJ) -o $(TARGET)

edhd.o : edhd.c
	$(CC) $(FLAGS) -c -o $@ $<
