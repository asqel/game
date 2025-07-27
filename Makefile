SRC = $(wildcard src/*.c src/*/*.c src/*/*/*.c) main.c
OBJ = ${SRC:.c=.o}

CC = gcc
LD = gcc

CFLAGS = -Wall -Wextra -g -I. #-fsanitize=address
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf #-fsanitize=address

NAME = game

all: $(NAME)

$(NAME): $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: re fclean clean all
