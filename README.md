# Tetris project

A modern **Tetris** clone written in **C++17** using **SDL3**.  
Implements the full gameplay loop, original Nintendo scoring system, 7-bag piece generation, Super Rotation System (known as SRS), level progression, and smooth gravity behavior.

### Features:
- ✅ **7-bag randomizer** — fair piece generation (no repeats until all 7 appear)
- ✅ **Normal rotation system** — rotation with collision detection
- ✅ **Hold delay / Lock delay**
- ✅ **Line clearing** and Nintendo-accurate scoring
- ✅ **Hard drop**
- ✅ **Dynamic difficulty progression** (Guideline gravity curve)
- ⚙️ **Minimal preprocessor logger** for debugging
- 🔲 **Pause menu** — *in progress*

---

### Bugs (TODO)
- [x] main.cpp (at the end) => Exception Thrown Run-Time Check Failure #2 - Stack around the variable 'game' was corrupted.
- [x] renderer.h - free memory at destructor
- [x] Game over! Press R to Restart => not restarting
- [ ] **Input quirk**: if you hold `R` during the Game Over screen, Minos freeze (might become a feature 😉)

### 🧱 Code Style

- All member variables prefixed with `m_`
- Bracket style: **1TBS (One True Brace Style)**
- Uses **C++17**
- Consistent `clang-format` style (Google/LLVM)
- Core architecture:
  - `Game` — main loop and state manager  
  - `Bag7` — mechanism for determining next piece type
  - `InputHandler` — class handling all the inputs from player
  - `Grid` — board representation  
  - `Mino` — tetromino pieces  
  - `HUD` — score, level, next piece  
  - `ScoreSystem` — score, level, next piece  
  - `Renderer` — SDL3 abstraction layer

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

### Code style:
- all member variables should start with m_ 
- 1TBS bracket style
