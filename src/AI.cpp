#include "../include/AI.h"
#include <algorithm>
#include <random>
#include <climits>

// Position bonus tables for different pieces (simplified)
const int AI::PAWN_POSITION_BONUS[8][8] = {
    {0,  0,  0,  0,  0,  0,  0,  0},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {10, 10, 20, 30, 30, 20, 10, 10},
    {5,  5, 10, 25, 25, 10,  5,  5},
    {0,  0,  0, 20, 20,  0,  0,  0},
    {5, -5,-10,  0,  0,-10, -5,  5},
    {5, 10, 10,-20,-20, 10, 10,  5},
    {0,  0,  0,  0,  0,  0,  0,  0}
};

const int AI::KNIGHT_POSITION_BONUS[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50}
};

const int AI::BISHOP_POSITION_BONUS[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20}
};

const int AI::ROOK_POSITION_BONUS[8][8] = {
    {0,  0,  0,  0,  0,  0,  0,  0},
    {5, 10, 10, 10, 10, 10, 10,  5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {0,  0,  0,  5,  5,  0,  0,  0}
};

const int AI::QUEEN_POSITION_BONUS[8][8] = {
    {-20,-10,-10, -5, -5,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5,  5,  5,  5,  0,-10},
    {-5,  0,  5,  5,  5,  5,  0, -5},
    {0,  0,  5,  5,  5,  5,  0, -5},
    {-10,  5,  5,  5,  5,  5,  0,-10},
    {-10,  0,  5,  0,  0,  0,  0,-10},
    {-20,-10,-10, -5, -5,-10,-10,-20}
};

const int AI::KING_POSITION_BONUS[8][8] = {
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    {20, 20,  0,  0,  0,  0, 20, 20},
    {20, 30, 10,  0,  0, 10, 30, 20}
};

// Constructor
AI::AI(AILevel level, Color color) : difficulty(level), aiColor(color) {}

// Main AI method - returns the best move
Move AI::getBestMove(const Board& board) {
    std::vector<Move> legalMoves = board.getAllLegalMoves(aiColor);
    
    if (legalMoves.empty()) {
        // This shouldn't happen in a valid game state
        return Move(0, 0, 0, 0);
    }
    
    switch (difficulty) {
        case AILevel::EASY:
            return getRandomMove(board);
            
        case AILevel::MEDIUM:
        case AILevel::HARD: {
            int maxDepth = (difficulty == AILevel::MEDIUM) ? 2 : 3;
            
            Move bestMove = legalMoves[0];
            int bestScore = INT_MIN;
            
            // Order moves for better pruning
            orderMoves(legalMoves, board);
            
            for (const Move& move : legalMoves) {
                // Make a copy of the board to test the move
                Board testBoard = board;
                testBoard.makeMove(move);
                
                // Use minimax to evaluate the position
                int score = minimax(testBoard, maxDepth - 1, false, INT_MIN, INT_MAX);
                
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = move;
                }
            }
            
            return bestMove;
        }
    }
    
    return legalMoves[0];  // Fallback
}

// Minimax algorithm with alpha-beta pruning
int AI::minimax(Board& board, int depth, bool isMaximizing, int alpha, int beta) {
    // Base case: reached maximum depth or game over
    if (depth == 0) {
        return evaluateBoard(board);
    }
    
    Color currentPlayer = board.getGameState().currentPlayer;
    std::vector<Move> moves = board.getAllLegalMoves(currentPlayer);
    
    // Check for game-ending conditions
    if (moves.empty()) {
        if (board.isInCheck(currentPlayer)) {
            // Checkmate - very bad for the player in check
            return isMaximizing ? INT_MIN + 1000 : INT_MAX - 1000;
        } else {
            // Stalemate - neutral
            return 0;
        }
    }
    
    if (board.isDraw()) {
        return 0;  // Draw is neutral
    }
    
    // Order moves for better pruning
    orderMoves(moves, board);
    
    if (isMaximizing) {
        int maxEval = INT_MIN;
        
        for (const Move& move : moves) {
            Board testBoard = board;
            testBoard.makeMove(move);
            
            int eval = minimax(testBoard, depth - 1, false, alpha, beta);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            
            if (beta <= alpha) {
                break;  // Alpha-beta pruning
            }
        }
        
        return maxEval;
    } else {
        int minEval = INT_MAX;
        
        for (const Move& move : moves) {
            Board testBoard = board;
            testBoard.makeMove(move);
            
            int eval = minimax(testBoard, depth - 1, true, alpha, beta);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            
            if (beta <= alpha) {
                break;  // Alpha-beta pruning
            }
        }
        
        return minEval;
    }
}

// Evaluate the board position
int AI::evaluateBoard(const Board& board) const {
    int score = 0;
    
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const Piece& piece = board.getPiece(row, col);
            if (!piece.isEmpty()) {
                int pieceValue = piece.getValue();
                int positionBonus = evaluatePiecePosition(piece.getType(), piece.getColor(), row, col);
                
                int totalValue = pieceValue + positionBonus;
                
                if (piece.getColor() == Color::WHITE) {
                    score += totalValue;
                } else {
                    score -= totalValue;
                }
            }
        }
    }
    
    // Adjust score based on AI color
    if (aiColor == Color::BLACK) {
        score = -score;
    }
    
    return score;
}

// Evaluate piece position bonus
int AI::evaluatePiecePosition(PieceType piece, Color color, int row, int col) const {
    int bonus = 0;
    int adjustedRow = (color == Color::BLACK) ? 7 - row : row;  // Flip for black pieces
    
    switch (piece) {
        case PieceType::PAWN:
            bonus = PAWN_POSITION_BONUS[adjustedRow][col];
            break;
        case PieceType::KNIGHT:
            bonus = KNIGHT_POSITION_BONUS[adjustedRow][col];
            break;
        case PieceType::BISHOP:
            bonus = BISHOP_POSITION_BONUS[adjustedRow][col];
            break;
        case PieceType::ROOK:
            bonus = ROOK_POSITION_BONUS[adjustedRow][col];
            break;
        case PieceType::QUEEN:
            bonus = QUEEN_POSITION_BONUS[adjustedRow][col];
            break;
        case PieceType::KING:
            bonus = KING_POSITION_BONUS[adjustedRow][col];
            break;
        default:
            bonus = 0;
    }
    
    return bonus;
}

// Order moves for better alpha-beta pruning (captures first)
void AI::orderMoves(std::vector<Move>& moves, const Board& board) const {
    std::sort(moves.begin(), moves.end(), [&board](const Move& a, const Move& b) {
        const Piece& targetA = board.getPiece(a.toRow, a.toCol);
        const Piece& targetB = board.getPiece(b.toRow, b.toCol);
        
        // Prioritize captures
        if (!targetA.isEmpty() && targetB.isEmpty()) return true;
        if (targetA.isEmpty() && !targetB.isEmpty()) return false;
        
        // If both are captures, prioritize higher value captures
        if (!targetA.isEmpty() && !targetB.isEmpty()) {
            return targetA.getValue() > targetB.getValue();
        }
        
        return false;  // Equal priority
    });
}

// Get random legal move (for easy difficulty)
Move AI::getRandomMove(const Board& board) const {
    std::vector<Move> legalMoves = board.getAllLegalMoves(aiColor);
    
    if (legalMoves.empty()) {
        return Move(0, 0, 0, 0);  // No legal moves
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, legalMoves.size() - 1);
    
    return legalMoves[dis(gen)];
}

// Check if a move is a capture
bool AI::isCapture(const Move& move, const Board& board) const {
    const Piece& target = board.getPiece(move.toRow, move.toCol);
    return !target.isEmpty();
}
