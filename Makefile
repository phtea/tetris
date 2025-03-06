# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -g

# SDL2 flags
SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LDFLAGS := $(shell sdl2-config --libs)

# Source files
SRC := main.cpp game.cpp renderer.cpp tetromino.cpp
OBJ := $(SRC:.cpp=.o)
TARGET := tetris

# Build rules
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(SDL2_LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SDL2_CFLAGS) -c $< -o $@

# Utility commands
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

debug: CXXFLAGS += -DDEBUG -O0
debug: clean $(TARGET)
	gdb ./$(TARGET)

.PHONY: all clean run debug
