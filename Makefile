SRC = $(wildcard src/*.c src/*/*.c src/*/*/*.c) main.c
OBJ = $(SRC:.c=.o)
OBJ := $(OBJ:src/%=%)
OBJ := $(addprefix obj/, $(OBJ))

CC = gcc
LD = gcc

CFLAGS = -Wall -Wextra -g -Iinclude/ $(SDL_INC) #-fsanitize=address
LDFLAGS = $(SDL_LIB) -lSDL2 -lSDL2_image -lSDL2_ttf -Llibs/ -llua54 -lm #-fsanitize=address
LIBS = 

ifeq ($(OS), Windows_NT)
	CFLAGS += -Ilua/win -DSDL_MAIN_HANDLED=1
	LIBS += $(wildcard libs/*.dll)
else
	CFLAGS += -Ilua/linux
endif

EXPORT_PATH = ./out/

NAME = game

all: $(NAME)

$(NAME): $(OBJ) $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.c
	@echo compiling src/$<...
	@mkdir -p $(dir $@)
	@$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean
	make -j10

export: $(NAME)
	mkdir -p $(EXPORT_PATH)
	cp $(NAME)* $(EXPORT_PATH)
	cp scripts/ -r $(EXPORT_PATH)/
	cp assets/ -r $(EXPORT_PATH)/
	cp lang/ -r $(EXPORT_PATH)
	cp libs/* $(EXPORT_PATH)
	cp world -r $(EXPORT_PATH)

	

.PHONY: re fclean clean all
