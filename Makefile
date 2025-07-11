SRC = $(wildcard src/*.c src/*/*.c src/*/*/*.c)
OBJ = ${SRC:.c=.o}

CC = gcc
LD = gcc

CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

NAME = game

all: $(NAME)

$(NAME): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CLFAGS) -c $< -o $@

clean:
	rm $(OBJ)

fclean: clean
	rm $(NAME)

re: fclean all

.PHONY: re fclean clean all
