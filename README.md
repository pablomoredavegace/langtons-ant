# Langton's Ant — Personal Implementation

This repository contains my implementation of Langton’s Ant in C++. It was developed as a small project to explore user-defined data types, separation of responsibilities, and basic simulation/visualization in the terminal.

## Overview

Langton’s Ant is a simple two-dimensional cellular automaton. An agent (the "ant") moves on a grid of binary cells (white or black). On each step the ant:
- If on a white cell: flip it to black, turn left 90°, move forward one cell.
- If on a black cell: flip it to white, turn right 90°, move forward one cell.

Despite these trivial rules, the system produces complex emergent behaviour (transient symmetry, chaotic region, and eventually a repeating "highway").

This project implements a text-mode simulator with:
- A `Tape` class that stores the grid and handles display.
- An `Ant` class that stores position and orientation and performs moves.
- A `Simulator` that coordinates the simulation loop, input/output, and state saving.

## Features

- Fixed-size 2D tape with configurable `sizeX` and `sizeY`.
- Two tape initialization modes:
  - All-white default.
  - Read coordinates of black cells from a text file.
- Text-mode visualization (space `' '` for white, `X` for black). Optional ANSI color support.
- Ant orientation displayed as `<`, `>`, `^`, `v`.
- Interactive step mode and run-for-N-steps mode.
- Save / load simulation state using the same simple text format.

## Requirements

- C++17-capable compiler (g++ / clang++)
- CMake (recommended)
- (Optional) A terminal that supports ANSI escape sequences for colored output

## Input / Save file format

The simulator reads (and can save) the simulation state in a simple text format:

- Line 1: tape size — two integers: `sizeX sizeY`
- Line 2: initial ant position and orientation — three integers: `x y orientation`
  - orientation: `0 = Left`, `1 = Right`, `2 = Up`, `3 = Down`
- Lines 3..n: coordinates of black cells — one pair per line: `x y`

Example `sample_input.txt`:

```
100 100
50 50 2
10 10
11 10
12 10
```

Meaning: grid 100×100, ant at (50,50) facing Up (`2`), and three black cells at (10,10), (11,10), (12,10).

## Contact

If you find issues or want improvements, open an issue or contact me at pablomoredavegace (GitHub).

---

If you want, I can:
- Generate a ready-to-use `CMakeLists.txt` and a minimal `main.cpp` that parses the input file and runs the simulation.
- Create a sample `sample_input.txt` and an initial `.gitignore` and `LICENSE` (MIT) file. Which of those should I create next?
