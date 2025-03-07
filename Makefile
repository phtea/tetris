.PHONY: all build run clean

all: build

build:
	cmake -B build -S .
	cmake --build build

run: build
	./build/tetris

clean:
	rm -rf build
