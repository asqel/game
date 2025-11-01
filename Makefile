SRC = $(wildcard src/*.c src/*/*.c src/*/*/*.c) main.c
OBJ = ${SRC:.c=.o}

CC = gcc
LD = gcc

CFLAGS = -Wall -Wextra -g -Iinclude/ #-fsanitize=address
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf #-fsanitize=address
LIBS = 
LDFLAGS += -Llibs/ -llua54 -lm
ifeq ($(OS), Windows_NT)
	CFLAGS += -Ilua/win -DSDL_MAIN_HANDLED=1
	LIBS += $(wildcard libs/*.dll)
else
	CFLAGS += -Ilua/linux
endif

NAME = game

all: $(NAME)

$(NAME): $(OBJ) $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: re fclean clean all
