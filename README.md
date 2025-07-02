# Tetris project

### Features:
- [x] hold delay
- [x] lock delay
- [x] make NORMAL rotate (with collision detection)
- [x] random piece generation system (7 dice bag)
- [x] clear row
- [ ] difficulty progression: game should become faster with time
- [x] hard drop
- [ ] pause menu
- [x] Preprosessor Logger (minimal)

---

### 🛠 How to Build on Linux

1. **Install SDL3 and related libraries** to your user directory:

Cd into any download directory and run:
```bash
git clone https://github.com/libsdl-org/SDL.git
cd SDL && mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/.local
make -j$(nproc) && make install

cd ../..
git clone https://github.com/libsdl-org/SDL_image.git
cd SDL_image && mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/.local
make -j$(nproc) && make install

cd ../..
git clone https://github.com/libsdl-org/SDL_ttf.git
cd SDL_ttf && mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/.local
make -j$(nproc) && make install
```

2. **Build the project:**

Run this command if just cloned
```bash
make init
```

Build in debug mode:
```bash
make debug
```
Release mode:
```bash
make release
```

3. **Run the game:**

```bash
./build/tetris
```

---

### Bugs (TODO)
- [ ] main.cpp (at the end) => Exception Thrown Run-Time Check Failure #2 - Stack around the variable 'game' was corrupted.
- [x] renderer.h - free memory at destructor
- [ ] Game over! Press R to Restart => not restarting

---

### Code style:
- all member variables should start with m_ 
- 1TBS bracket style
