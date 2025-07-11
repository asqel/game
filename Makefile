SRC = $(wildcard src/*.c src/*/*.c src/*/*/*.c)
OBJ = ${SRC:.c=.o}

CC = gcc
LD = gcc

CFLAGS = 

NAME = game

all: $(NAME)

$(NAME):
	
