## Tetris project
Features:
- [x] hold delay
- [ ] lock delay
- [x] make NORMAL rotate (with collision detection)
- [ ] random piece generation system (7 dice bag)
- [ ] clear row
- [ ] difficulty progression: game should become faster with time
- [ ] hard drop feature
- [ ] pause menu

- [ ] Fixes:
- [x] renderer.h - free memory at destructor

Architecture:
- [x] improve input handling architecture: change keyStates and justPressed to something else (there has to be a better way)
=> what done: created a 
- [ ] map of keyStates which now holds enum of different keyStates (subjectively better)

Code style:
- all member variables should start with m_ 
- 1TBS bracket style