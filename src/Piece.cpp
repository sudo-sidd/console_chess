#include "../include/Piece.h"
#include <cmath>

// Constructor implementation
Piece::Piece(PieceType t, Color c) : type(t), color(c) {}

// Get Unicode symbol for piece display
std::string Piece::getSymbol() const {
    if (type == PieceType::EMPTY) {
        return " ";
    }
    
    // Unicode chess pieces - this is a great C++ feature for international text
    if (color == Color::WHITE) {
        switch (type) {
            case PieceType::KING:   return "♔";
            case PieceType::QUEEN:  return "♕";
            case PieceType::ROOK:   return "♖";
            case PieceType::BISHOP: return "♗";
            case PieceType::KNIGHT: return "♘";
            case PieceType::PAWN:   return "♙";
            default: return " ";
        }
    } else {
        switch (type) {
            case PieceType::KING:   return "♚";
            case PieceType::QUEEN:  return "♛";
            case PieceType::ROOK:   return "♜";
            case PieceType::BISHOP: return "♝";
            case PieceType::KNIGHT: return "♞";
            case PieceType::PAWN:   return "♟";
            default: return " ";
        }
    }
}

// Get piece value for AI evaluation
int Piece::getValue() const {
    switch (type) {
        case PieceType::PAWN:   return 100;
        case PieceType::KNIGHT: return 320;
        case PieceType::BISHOP: return 330;
        case PieceType::ROOK:   return 500;
        case PieceType::QUEEN:  return 900;
        case PieceType::KING:   return 20000;
        default: return 0;
    }
}

// Basic movement rules for each piece (without board context)
bool Piece::canMoveTo(int fromRow, int fromCol, int toRow, int toCol) const {
    if (type == PieceType::EMPTY) return false;
    
    // Can't move to same square
    if (fromRow == toRow && fromCol == toCol) return false;
    
    int deltaRow = toRow - fromRow;
    int deltaCol = toCol - fromCol;
    int absDeltaRow = std::abs(deltaRow);
    int absDeltaCol = std::abs(deltaCol);
    
    switch (type) {
        case PieceType::PAWN: {
            // Pawns are complex due to direction, initial move, and capture rules
            int direction = (color == Color::WHITE) ? -1 : 1;  // White moves up (decreasing row)
            
            // Forward move
            if (deltaCol == 0) {
                if (deltaRow == direction) return true;  // One square forward
                // Two squares from starting position
                if ((color == Color::WHITE && fromRow == 6 && deltaRow == -2) ||
                    (color == Color::BLACK && fromRow == 1 && deltaRow == 2)) {
                    return true;
                }
            }
            // Diagonal capture
            else if (absDeltaCol == 1 && deltaRow == direction) {
                return true;  // Board will check if there's a piece to capture
            }
            return false;
        }
        
        case PieceType::ROOK:
            // Rooks move in straight lines
            return (deltaRow == 0) != (deltaCol == 0);  // XOR: exactly one should be 0
        
        case PieceType::BISHOP:
            // Bishops move diagonally
            return absDeltaRow == absDeltaCol;
        
        case PieceType::QUEEN:
            // Queens combine rook and bishop movement
            return (deltaRow == 0) || (deltaCol == 0) || (absDeltaRow == absDeltaCol);
        
        case PieceType::KNIGHT:
            // Knights move in L-shape: 2+1 or 1+2
            return (absDeltaRow == 2 && absDeltaCol == 1) || (absDeltaRow == 1 && absDeltaCol == 2);
        
        case PieceType::KING:
            // Kings move one square in any direction
            return absDeltaRow <= 1 && absDeltaCol <= 1;
        
        default:
            return false;
    }
}
