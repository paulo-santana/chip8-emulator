NAME := ch8

SRC_DIR := ./src
OBJ_DIR ?= ./obj

SOURCES := main.cpp \
					 chip8.cpp \
					 window.cpp \
					 opcodes.cpp \
					 font.cpp

OBJECTS := $(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o))

DEPS := $(OBJECTS:.o=.d)

CXXFLAGS += $(shell pkg-config --cflags sdl3)
CXXFLAGS += -std=c++20 -Wall -Wextra -g -MMD -MP
LDLIBS += $(shell pkg-config --libs sdl3)

all: $(NAME)

run: $(NAME)
	./$(NAME) "../roms/games/Pong (1 player).ch8"

test1: $(NAME)
	./$(NAME) "../roms/test-suite/1-chip8-logo.ch8"

test2: $(NAME)
	./$(NAME) "../roms/test-suite/2-ibm-logo.ch8"

picture: $(NAME)
	./$(NAME) "../roms/programs/Chip8 Picture.ch8"

mirror: $(NAME)
	./$(NAME) "../roms/games/X-Mirror.ch8"

fishie: $(NAME)
	./$(NAME) "../roms/programs/Fishie [Hap, 2005].ch8"

vers: $(NAME)
	./$(NAME) "../roms/games/Vers [JMN, 1991].ch8"

random: $(NAME)
	./$(NAME) "../roms/programs/Random Number Test [Matthew Mikolay, 2010].ch8"

rm:
	rm -rf $(OBJ_DIR)

clean: rm
	rm -f $(NAME)

re: rm all

$(NAME): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDLIBS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

-include $(DEPS)
