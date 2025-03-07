.PHONY: all out run clean

all: out

out:
	cmake -B out -S .
	cmake --build out

run: out
	./out/tetris

clean:
	rm -rf out
