NAME := ch8

SRC_DIR := ./src
OBJ_DIR ?= ./obj

SOURCES := main.cpp \
					 chip8.cpp

OBJECTS := $(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o))

CXXFLAGS += $(shell pkg-config --cflags sdl3)
CXXFLAGS += -std=c++17 -Wall -Wextra
LDLIBS += $(shell pkg-config --libs sdl3)

all: $(NAME)

run: $(NAME)
	./$(NAME) "../roms/games/Pong (1 player).ch8"

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
