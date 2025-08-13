#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <vector>

// Enum for piece colors - in C++, enums help with type safety
enum class Color {
    WHITE,
    BLACK,
    NONE  // For empty squares
};

// Enum for piece types
enum class PieceType {
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
    EMPTY  // For empty squares
};

// Structure to represent a move - C++ structs are like classes but public by default
struct Move {
    int fromRow, fromCol;
    int toRow, toCol;
    bool isCapture;
    bool isCastling;
    bool isEnPassant;
    PieceType promotionPiece;  // For pawn promotion
    
    // Constructor - C++ allows multiple constructors
    Move(int fr, int fc, int tr, int tc) 
        : fromRow(fr), fromCol(fc), toRow(tr), toCol(tc), 
          isCapture(false), isCastling(false), isEnPassant(false), 
          promotionPiece(PieceType::EMPTY) {}
};

// Class representing a chess piece
class Piece {
private:
    PieceType type;
    Color color;

public:
    // Constructor
    Piece(PieceType t = PieceType::EMPTY, Color c = Color::NONE);
    
    // Getters - const means these methods don't modify the object
    PieceType getType() const { return type; }
    Color getColor() const { return color; }
    
    // Check if piece is empty
    bool isEmpty() const { return type == PieceType::EMPTY; }
    
    // Get Unicode symbol for display
    std::string getSymbol() const;
    
    // Get piece value for AI evaluation
    int getValue() const;
    
    // Check if this piece can move to target position (basic rules, no board context)
    bool canMoveTo(int fromRow, int fromCol, int toRow, int toCol) const;
};

#endif // PIECE_H
