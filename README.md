# Console Chess Game

A full-featured chess game written in C++ that runs in the terminal.

## Features

- **Offline 2-player turn-based mode**: Play against a friend locally
- **Simple AI opponent**: Choose to play against a rule-based AI with minimax algorithm
- **Beautiful terminal display**:

  - Horizontally centered, large chess board
  - Unicode chess pieces with proper alignment
  - Clean box-drawing borders and grid
  - Properly aligned coordinate labels (a-h, 1-8)
- **Complete chess rules**: Including castling, en passant, pawn promotion
- **Game state management**: Proper tracking of all chess rules and conditions

## Recent Improvements

✅ **Enhanced Board Display**:

- Horizontally centered board for better visual appeal
- Larger squares for improved readability  
- Perfect alignment of file letters (a-h) and rank numbers (1-8)
- Professional box-drawing characters for clean borders
- Consistent spacing and formatting

## How to Build

```bash
# Compile the game
g++ -std=c++17 -I. main.cpp src/*.cpp -o chess_game

# Run the game
./chess_game
```

## How to Play

1. Launch the game and select your game mode:
   - Player vs Player
   - Player vs AI (you play as White)
   - Player vs AI (you play as Black)

2. Move pieces using algebraic notation:
   - Format: `e2 e4` (from square to square)
   - Examples: `e2 e4`, `g1 f3`, `e1 g1` (castling)

3. Special moves:
   - **Castling**: Move king two squares toward rook
   - **En passant**: Automatic when conditions are met
   - **Pawn promotion**: Prompted when pawn reaches end rank

## Project Structure

```text
console_chess/
├── main.cpp              # Entry point
├── include/              # Header files
│   ├── Piece.h
│   ├── Board.h
│   ├── Game.h
│   ├── AI.h
│   └── Utils.h
├── src/                  # Implementation files
│   ├── Piece.cpp
│   ├── Board.cpp
│   ├── Game.cpp
│   ├── AI.cpp
│   └── Utils.cpp
└── README.md
```

## Learning C++ Through This Project

This project demonstrates many important C++ concepts:

- **Classes and Objects**: Game, Board, Piece classes
- **Enums**: For piece types and colors
- **Vectors**: For storing moves and board state
- **References and Pointers**: Efficient parameter passing
- **Standard Library**: Using STL containers and algorithms
- **Memory Management**: RAII principles
- **Object-Oriented Design**: Encapsulation and abstraction
