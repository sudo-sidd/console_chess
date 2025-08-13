#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include <vector>
#include <string>

// Structure to track game state - important for chess rules
struct GameState {
    Color currentPlayer;
    bool whiteCanCastleKingside;
    bool whiteCanCastleQueenside;
    bool blackCanCastleKingside;
    bool blackCanCastleQueenside;
    int enPassantCol;  // -1 if no en passant possible
    int halfMoveClock; // For 50-move rule
    int fullMoveNumber;
    
    // Constructor with default values
    GameState() : currentPlayer(Color::WHITE), 
                  whiteCanCastleKingside(true), whiteCanCastleQueenside(true),
                  blackCanCastleKingside(true), blackCanCastleQueenside(true),
                  enPassantCol(-1), halfMoveClock(0), fullMoveNumber(1) {}
};

class Board {
private:
    // 8x8 board represented as vector of vectors - C++ containers are powerful!
    std::vector<std::vector<Piece>> board;
    GameState gameState;
    
    // Helper methods (private implementation details)
    bool isPathClear(int fromRow, int fromCol, int toRow, int toCol) const;
    bool wouldBeInCheck(Color color, const Move& move) const;
    void findKing(Color color, int& kingRow, int& kingCol) const;
    void updateGameState(const Move& move, const Piece& movingPiece, const Piece& capturedPiece);

public:
    // Constructor - initializes board to starting position
    Board();
    
    // Board access methods
    const Piece& getPiece(int row, int col) const;
    void setPiece(int row, int col, const Piece& piece);
    
    // Game state access
    const GameState& getGameState() const { return gameState; }
    void setCurrentPlayer(Color player) { gameState.currentPlayer = player; }
    
    // Move validation and execution
    bool isValidMove(const Move& move) const;
    bool makeMove(const Move& move);  // Returns true if move was successful
    
    // Game state checking
    bool isInCheck(Color color) const;
    bool isCheckmate(Color color) const;
    bool isStalemate(Color color) const;
    bool isDraw() const;  // 50-move rule, insufficient material, etc.
    
    // Move generation
    std::vector<Move> getAllLegalMoves(Color color) const;
    std::vector<Move> getPossibleMoves(int row, int col) const;
    
    // Display
    void display() const;
    std::string getSquareName(int row, int col) const;  // e.g., "e4"
    
    // Utility methods
    bool isOnBoard(int row, int col) const;
    void resetToStartingPosition();
    
    // For AI evaluation
    int evaluatePosition() const;  // Positive for white advantage
};

#endif // BOARD_H
