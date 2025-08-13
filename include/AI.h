#ifndef AI_H
#define AI_H

#include "Board.h"
#include "Piece.h"
#include <vector>

// AI difficulty levels
enum class AILevel {
    EASY,    // Random moves
    MEDIUM,  // Minimax depth 2
    HARD     // Minimax depth 3
};

class AI {
private:
    AILevel difficulty;
    Color aiColor;
    
    // Minimax algorithm with alpha-beta pruning
    int minimax(Board& board, int depth, bool isMaximizing, int alpha, int beta);
    
    // Evaluation function
    int evaluateBoard(const Board& board) const;
    int evaluatePiecePosition(PieceType piece, Color color, int row, int col) const;
    
    // Move ordering for better alpha-beta pruning
    void orderMoves(std::vector<Move>& moves, const Board& board) const;
    
    // Utility methods
    Move getRandomMove(const Board& board) const;
    bool isCapture(const Move& move, const Board& board) const;

public:
    // Constructor
    AI(AILevel level = AILevel::MEDIUM, Color color = Color::BLACK);
    
    // Main AI method - returns the best move
    Move getBestMove(const Board& board);
    
    // Getters and setters
    AILevel getDifficulty() const { return difficulty; }
    void setDifficulty(AILevel level) { difficulty = level; }
    
    Color getColor() const { return aiColor; }
    void setColor(Color color) { aiColor = color; }
    
    // Evaluation constants
    static const int PAWN_VALUE = 100;
    static const int KNIGHT_VALUE = 320;
    static const int BISHOP_VALUE = 330;
    static const int ROOK_VALUE = 500;
    static const int QUEEN_VALUE = 900;
    static const int KING_VALUE = 20000;
    
    // Position bonus tables (simplified)
    static const int PAWN_POSITION_BONUS[8][8];
    static const int KNIGHT_POSITION_BONUS[8][8];
    static const int BISHOP_POSITION_BONUS[8][8];
    static const int ROOK_POSITION_BONUS[8][8];
    static const int QUEEN_POSITION_BONUS[8][8];
    static const int KING_POSITION_BONUS[8][8];
};

#endif // AI_H
