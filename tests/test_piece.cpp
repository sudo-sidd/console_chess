#include "test_framework.h"
#include "../include/Piece.h"
#include <iostream>

void test_piece_construction() {
    // Test default constructor
    Piece empty_piece;
    TestFramework::assert_true(empty_piece.isEmpty(), "Default piece is empty");
    TestFramework::assert_equal(static_cast<int>(PieceType::EMPTY), static_cast<int>(empty_piece.getType()), "Default piece type is EMPTY");
    TestFramework::assert_equal(static_cast<int>(Color::NONE), static_cast<int>(empty_piece.getColor()), "Default piece color is NONE");
    
    // Test parameterized constructor
    Piece white_king(PieceType::KING, Color::WHITE);
    TestFramework::assert_true(!white_king.isEmpty(), "White king is not empty");
    TestFramework::assert_equal(static_cast<int>(PieceType::KING), static_cast<int>(white_king.getType()), "White king type is KING");
    TestFramework::assert_equal(static_cast<int>(Color::WHITE), static_cast<int>(white_king.getColor()), "White king color is WHITE");
    
    Piece black_pawn(PieceType::PAWN, Color::BLACK);
    TestFramework::assert_equal(static_cast<int>(PieceType::PAWN), static_cast<int>(black_pawn.getType()), "Black pawn type is PAWN");
    TestFramework::assert_equal(static_cast<int>(Color::BLACK), static_cast<int>(black_pawn.getColor()), "Black pawn color is BLACK");
}

void test_piece_values() {
    Piece pawn(PieceType::PAWN, Color::WHITE);
    Piece rook(PieceType::ROOK, Color::WHITE);
    Piece knight(PieceType::KNIGHT, Color::WHITE);
    Piece bishop(PieceType::BISHOP, Color::WHITE);
    Piece queen(PieceType::QUEEN, Color::WHITE);
    Piece king(PieceType::KING, Color::WHITE);
    Piece empty;
    
    TestFramework::assert_equal(100, pawn.getValue(), "Pawn value is 100 centipawns");
    TestFramework::assert_equal(500, rook.getValue(), "Rook value is 500 centipawns");
    TestFramework::assert_equal(320, knight.getValue(), "Knight value is 320 centipawns");
    TestFramework::assert_equal(330, bishop.getValue(), "Bishop value is 330 centipawns");
    TestFramework::assert_equal(900, queen.getValue(), "Queen value is 900 centipawns");
    TestFramework::assert_equal(20000, king.getValue(), "King value is 20000 centipawns");
    TestFramework::assert_equal(0, empty.getValue(), "Empty piece value is 0");
}

void test_piece_symbols() {
    Piece white_king(PieceType::KING, Color::WHITE);
    Piece black_king(PieceType::KING, Color::BLACK);
    Piece white_queen(PieceType::QUEEN, Color::WHITE);
    Piece black_queen(PieceType::QUEEN, Color::BLACK);
    Piece empty;
    
    // Test that symbols are not empty (actual Unicode symbols may vary)
    TestFramework::assert_true(!white_king.getSymbol().empty(), "White king has symbol");
    TestFramework::assert_true(!black_king.getSymbol().empty(), "Black king has symbol");
    TestFramework::assert_true(!white_queen.getSymbol().empty(), "White queen has symbol");
    TestFramework::assert_true(!black_queen.getSymbol().empty(), "Black queen has symbol");
    
    // Different pieces should have different symbols (for same color)
    TestFramework::assert_true(white_king.getSymbol() != white_queen.getSymbol(), "Different pieces have different symbols");
    
    // Empty piece should have space symbol
    TestFramework::assert_equal(" ", empty.getSymbol(), "Empty piece symbol is space");
}

void test_pawn_movement() {
    Piece white_pawn(PieceType::PAWN, Color::WHITE);
    Piece black_pawn(PieceType::PAWN, Color::BLACK);
    
    // White pawn basic moves (from row 6)
    TestFramework::assert_true(white_pawn.canMoveTo(6, 4, 5, 4), "White pawn can move one square forward");
    TestFramework::assert_true(white_pawn.canMoveTo(6, 4, 4, 4), "White pawn can move two squares from starting position");
    TestFramework::assert_true(white_pawn.canMoveTo(6, 4, 5, 3), "White pawn can capture diagonally left");
    TestFramework::assert_true(white_pawn.canMoveTo(6, 4, 5, 5), "White pawn can capture diagonally right");
    
    // White pawn invalid moves
    TestFramework::assert_true(!white_pawn.canMoveTo(6, 4, 7, 4), "White pawn cannot move backward");
    TestFramework::assert_true(!white_pawn.canMoveTo(6, 4, 6, 5), "White pawn cannot move sideways");
    TestFramework::assert_true(!white_pawn.canMoveTo(5, 4, 3, 4), "White pawn cannot move two squares except from starting position");
    
    // Black pawn basic moves (from row 1)
    TestFramework::assert_true(black_pawn.canMoveTo(1, 4, 2, 4), "Black pawn can move one square forward");
    TestFramework::assert_true(black_pawn.canMoveTo(1, 4, 3, 4), "Black pawn can move two squares from starting position");
    TestFramework::assert_true(black_pawn.canMoveTo(1, 4, 2, 3), "Black pawn can capture diagonally left");
    TestFramework::assert_true(black_pawn.canMoveTo(1, 4, 2, 5), "Black pawn can capture diagonally right");
    
    // Black pawn invalid moves
    TestFramework::assert_true(!black_pawn.canMoveTo(1, 4, 0, 4), "Black pawn cannot move backward");
    TestFramework::assert_true(!black_pawn.canMoveTo(2, 4, 4, 4), "Black pawn cannot move two squares except from starting position");
}

void test_rook_movement() {
    Piece rook(PieceType::ROOK, Color::WHITE);
    
    // Valid rook moves (horizontal and vertical)
    TestFramework::assert_true(rook.canMoveTo(4, 4, 4, 7), "Rook can move horizontally right");
    TestFramework::assert_true(rook.canMoveTo(4, 4, 4, 0), "Rook can move horizontally left");
    TestFramework::assert_true(rook.canMoveTo(4, 4, 0, 4), "Rook can move vertically up");
    TestFramework::assert_true(rook.canMoveTo(4, 4, 7, 4), "Rook can move vertically down");
    
    // Invalid rook moves (diagonal)
    TestFramework::assert_true(!rook.canMoveTo(4, 4, 5, 5), "Rook cannot move diagonally");
    TestFramework::assert_true(!rook.canMoveTo(4, 4, 3, 3), "Rook cannot move diagonally");
    TestFramework::assert_true(!rook.canMoveTo(4, 4, 6, 2), "Rook cannot move in L-shape");
}

void test_knight_movement() {
    Piece knight(PieceType::KNIGHT, Color::WHITE);
    
    // Valid knight moves (L-shaped)
    TestFramework::assert_true(knight.canMoveTo(4, 4, 6, 5), "Knight can move L-shape down-right");
    TestFramework::assert_true(knight.canMoveTo(4, 4, 6, 3), "Knight can move L-shape down-left");
    TestFramework::assert_true(knight.canMoveTo(4, 4, 2, 5), "Knight can move L-shape up-right");
    TestFramework::assert_true(knight.canMoveTo(4, 4, 2, 3), "Knight can move L-shape up-left");
    TestFramework::assert_true(knight.canMoveTo(4, 4, 5, 6), "Knight can move L-shape right-down");
    TestFramework::assert_true(knight.canMoveTo(4, 4, 3, 6), "Knight can move L-shape right-up");
    TestFramework::assert_true(knight.canMoveTo(4, 4, 5, 2), "Knight can move L-shape left-down");
    TestFramework::assert_true(knight.canMoveTo(4, 4, 3, 2), "Knight can move L-shape left-up");
    
    // Invalid knight moves
    TestFramework::assert_true(!knight.canMoveTo(4, 4, 4, 6), "Knight cannot move horizontally");
    TestFramework::assert_true(!knight.canMoveTo(4, 4, 6, 4), "Knight cannot move vertically");
    TestFramework::assert_true(!knight.canMoveTo(4, 4, 6, 6), "Knight cannot move diagonally");
    TestFramework::assert_true(!knight.canMoveTo(4, 4, 7, 7), "Knight cannot move like other pieces");
}

void test_bishop_movement() {
    Piece bishop(PieceType::BISHOP, Color::WHITE);
    
    // Valid bishop moves (diagonal)
    TestFramework::assert_true(bishop.canMoveTo(4, 4, 7, 7), "Bishop can move diagonally down-right");
    TestFramework::assert_true(bishop.canMoveTo(4, 4, 1, 1), "Bishop can move diagonally up-left");
    TestFramework::assert_true(bishop.canMoveTo(4, 4, 7, 1), "Bishop can move diagonally down-left");
    TestFramework::assert_true(bishop.canMoveTo(4, 4, 1, 7), "Bishop can move diagonally up-right");
    
    // Invalid bishop moves
    TestFramework::assert_true(!bishop.canMoveTo(4, 4, 4, 7), "Bishop cannot move horizontally");
    TestFramework::assert_true(!bishop.canMoveTo(4, 4, 7, 4), "Bishop cannot move vertically");
    TestFramework::assert_true(!bishop.canMoveTo(4, 4, 6, 5), "Bishop cannot move in L-shape");
}

void test_queen_movement() {
    Piece queen(PieceType::QUEEN, Color::WHITE);
    
    // Valid queen moves (combines rook and bishop)
    TestFramework::assert_true(queen.canMoveTo(4, 4, 4, 7), "Queen can move horizontally");
    TestFramework::assert_true(queen.canMoveTo(4, 4, 7, 4), "Queen can move vertically");
    TestFramework::assert_true(queen.canMoveTo(4, 4, 7, 7), "Queen can move diagonally");
    TestFramework::assert_true(queen.canMoveTo(4, 4, 1, 1), "Queen can move diagonally up-left");
    
    // Invalid queen moves
    TestFramework::assert_true(!queen.canMoveTo(4, 4, 6, 5), "Queen cannot move in L-shape");
}

void test_king_movement() {
    Piece king(PieceType::KING, Color::WHITE);
    
    // Valid king moves (one square in any direction)
    TestFramework::assert_true(king.canMoveTo(4, 4, 5, 4), "King can move one square down");
    TestFramework::assert_true(king.canMoveTo(4, 4, 3, 4), "King can move one square up");
    TestFramework::assert_true(king.canMoveTo(4, 4, 4, 5), "King can move one square right");
    TestFramework::assert_true(king.canMoveTo(4, 4, 4, 3), "King can move one square left");
    TestFramework::assert_true(king.canMoveTo(4, 4, 5, 5), "King can move one square diagonally");
    TestFramework::assert_true(king.canMoveTo(4, 4, 3, 3), "King can move one square diagonally");
    
    // Invalid king moves (more than one square)
    TestFramework::assert_true(!king.canMoveTo(4, 4, 6, 4), "King cannot move two squares");
    TestFramework::assert_true(!king.canMoveTo(4, 4, 4, 6), "King cannot move two squares horizontally");
    TestFramework::assert_true(!king.canMoveTo(4, 4, 6, 6), "King cannot move two squares diagonally");
}

void test_move_structure() {
    Move move1(2, 3, 4, 5);
    TestFramework::assert_equal(2, move1.fromRow, "Move fromRow correct");
    TestFramework::assert_equal(3, move1.fromCol, "Move fromCol correct");
    TestFramework::assert_equal(4, move1.toRow, "Move toRow correct");
    TestFramework::assert_equal(5, move1.toCol, "Move toCol correct");
    TestFramework::assert_true(!move1.isCapture, "Move default isCapture false");
    TestFramework::assert_true(!move1.isCastling, "Move default isCastling false");
    TestFramework::assert_true(!move1.isEnPassant, "Move default isEnPassant false");
    TestFramework::assert_equal(static_cast<int>(PieceType::EMPTY), static_cast<int>(move1.promotionPiece), "Move default promotionPiece EMPTY");
}

// Main function for standalone execution
int main() {
    std::cout << "Running Piece Tests" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    TestFramework::run_test("Piece Construction", test_piece_construction);
    TestFramework::run_test("Piece Values", test_piece_values);
    TestFramework::run_test("Piece Symbols", test_piece_symbols);
    TestFramework::run_test("Pawn Movement", test_pawn_movement);
    TestFramework::run_test("Rook Movement", test_rook_movement);
    TestFramework::run_test("Knight Movement", test_knight_movement);
    TestFramework::run_test("Bishop Movement", test_bishop_movement);
    TestFramework::run_test("Queen Movement", test_queen_movement);
    TestFramework::run_test("King Movement", test_king_movement);
    TestFramework::run_test("Move Structure", test_move_structure);
    
    TestFramework::print_summary();
    
    return TestFramework::all_tests_passed() ? 0 : 1;
}
