#include "test_framework.h"
#include "../include/Board.h"
#include <iostream>

void test_board_initialization() {
    Board board;
    
    // Test that board is properly initialized with starting position
    // White pieces on bottom rows (6,7)
    TestFramework::assert_equal(static_cast<int>(PieceType::PAWN), static_cast<int>(board.getPiece(6, 0).getType()), "White pawn at a2");
    TestFramework::assert_equal(static_cast<int>(Color::WHITE), static_cast<int>(board.getPiece(6, 0).getColor()), "White pawn color");
    
    TestFramework::assert_equal(static_cast<int>(PieceType::ROOK), static_cast<int>(board.getPiece(7, 0).getType()), "White rook at a1");
    TestFramework::assert_equal(static_cast<int>(PieceType::KNIGHT), static_cast<int>(board.getPiece(7, 1).getType()), "White knight at b1");
    TestFramework::assert_equal(static_cast<int>(PieceType::BISHOP), static_cast<int>(board.getPiece(7, 2).getType()), "White bishop at c1");
    TestFramework::assert_equal(static_cast<int>(PieceType::QUEEN), static_cast<int>(board.getPiece(7, 3).getType()), "White queen at d1");
    TestFramework::assert_equal(static_cast<int>(PieceType::KING), static_cast<int>(board.getPiece(7, 4).getType()), "White king at e1");
    
    // Black pieces on top rows (0,1)
    TestFramework::assert_equal(static_cast<int>(PieceType::PAWN), static_cast<int>(board.getPiece(1, 0).getType()), "Black pawn at a7");
    TestFramework::assert_equal(static_cast<int>(Color::BLACK), static_cast<int>(board.getPiece(1, 0).getColor()), "Black pawn color");
    
    TestFramework::assert_equal(static_cast<int>(PieceType::ROOK), static_cast<int>(board.getPiece(0, 0).getType()), "Black rook at a8");
    TestFramework::assert_equal(static_cast<int>(PieceType::KING), static_cast<int>(board.getPiece(0, 4).getType()), "Black king at e8");
    
    // Empty squares in the middle
    TestFramework::assert_true(board.getPiece(3, 3).isEmpty(), "Empty square at d5");
    TestFramework::assert_true(board.getPiece(4, 4).isEmpty(), "Empty square at e4");
    
    // Test initial game state
    TestFramework::assert_equal(static_cast<int>(Color::WHITE), static_cast<int>(board.getGameState().currentPlayer), "White starts first");
    TestFramework::assert_true(board.getGameState().whiteCanCastleKingside, "White can castle kingside initially");
    TestFramework::assert_true(board.getGameState().blackCanCastleQueenside, "Black can castle queenside initially");
    TestFramework::assert_equal(-1, board.getGameState().enPassantCol, "No en passant initially");
}

void test_board_utilities() {
    Board board;
    
    // Test isOnBoard
    TestFramework::assert_true(board.isOnBoard(0, 0), "a8 is on board");
    TestFramework::assert_true(board.isOnBoard(7, 7), "h1 is on board");
    TestFramework::assert_true(board.isOnBoard(3, 4), "e5 is on board");
    TestFramework::assert_true(!board.isOnBoard(-1, 0), "Row -1 is off board");
    TestFramework::assert_true(!board.isOnBoard(0, -1), "Col -1 is off board");
    TestFramework::assert_true(!board.isOnBoard(8, 0), "Row 8 is off board");
    TestFramework::assert_true(!board.isOnBoard(0, 8), "Col 8 is off board");
    
    // Test getSquareName
    TestFramework::assert_equal("a8", board.getSquareName(0, 0), "Square name a8");
    TestFramework::assert_equal("h1", board.getSquareName(7, 7), "Square name h1");
    TestFramework::assert_equal("e4", board.getSquareName(4, 4), "Square name e4");
    TestFramework::assert_equal("d5", board.getSquareName(3, 3), "Square name d5");
}

void test_basic_move_validation() {
    Board board;
    
    // Test basic pawn moves
    Move pawnMove1(6, 4, 5, 4);  // e2 to e3
    TestFramework::assert_true(board.isValidMove(pawnMove1), "Pawn can move one square forward");
    
    Move pawnMove2(6, 4, 4, 4);  // e2 to e4
    TestFramework::assert_true(board.isValidMove(pawnMove2), "Pawn can move two squares from starting position");
    
    Move invalidMove1(6, 4, 3, 4);  // e2 to e5 (too far)
    TestFramework::assert_true(!board.isValidMove(invalidMove1), "Pawn cannot move three squares");
    
    Move invalidMove2(6, 4, 6, 5);  // e2 to f2 (sideways)
    TestFramework::assert_true(!board.isValidMove(invalidMove2), "Pawn cannot move sideways");
    
    // Test moving empty square
    Move invalidMove3(3, 3, 4, 4);  // Empty square
    TestFramework::assert_true(!board.isValidMove(invalidMove3), "Cannot move empty square");
    
    // Test moving opponent's piece
    Move invalidMove4(1, 4, 2, 4);  // Black pawn when white's turn
    TestFramework::assert_true(!board.isValidMove(invalidMove4), "Cannot move opponent's piece");
}

void test_move_execution() {
    Board board;
    
    // Make a valid pawn move
    Move pawnMove(6, 4, 4, 4);  // e2 to e4
    TestFramework::assert_true(board.makeMove(pawnMove), "Pawn move executed successfully");
    
    // Check piece moved correctly
    TestFramework::assert_true(board.getPiece(6, 4).isEmpty(), "Original square is now empty");
    TestFramework::assert_equal(static_cast<int>(PieceType::PAWN), static_cast<int>(board.getPiece(4, 4).getType()), "Pawn moved to new square");
    TestFramework::assert_equal(static_cast<int>(Color::WHITE), static_cast<int>(board.getPiece(4, 4).getColor()), "Moved pawn is white");
    
    // Check turn changed
    TestFramework::assert_equal(static_cast<int>(Color::BLACK), static_cast<int>(board.getGameState().currentPlayer), "Turn changed to black");
    
    // Make black pawn move
    Move blackPawnMove(1, 4, 3, 4);  // e7 to e5
    TestFramework::assert_true(board.makeMove(blackPawnMove), "Black pawn move executed");
    TestFramework::assert_equal(static_cast<int>(Color::WHITE), static_cast<int>(board.getGameState().currentPlayer), "Turn changed back to white");
}

void test_piece_setting() {
    Board board;
    
    // Test setPiece
    Piece queen(PieceType::QUEEN, Color::WHITE);
    board.setPiece(3, 3, queen);
    
    TestFramework::assert_equal(static_cast<int>(PieceType::QUEEN), static_cast<int>(board.getPiece(3, 3).getType()), "Queen placed on board");
    TestFramework::assert_equal(static_cast<int>(Color::WHITE), static_cast<int>(board.getPiece(3, 3).getColor()), "Queen color correct");
    
    // Test replacing piece
    Piece knight(PieceType::KNIGHT, Color::BLACK);
    board.setPiece(3, 3, knight);
    TestFramework::assert_equal(static_cast<int>(PieceType::KNIGHT), static_cast<int>(board.getPiece(3, 3).getType()), "Knight replaced queen");
    TestFramework::assert_equal(static_cast<int>(Color::BLACK), static_cast<int>(board.getPiece(3, 3).getColor()), "Knight color correct");
}

void test_check_detection() {
    Board board;
    
    // Initially, neither king should be in check
    TestFramework::assert_true(!board.isInCheck(Color::WHITE), "White king not in check initially");
    TestFramework::assert_true(!board.isInCheck(Color::BLACK), "Black king not in check initially");
    
    // Create a simple check scenario
    // Clear some pieces and place attacking piece
    board.setPiece(1, 4, Piece()); // Remove black pawn
    board.setPiece(2, 4, Piece()); // Clear path
    board.setPiece(3, 4, Piece()); // Clear path
    board.setPiece(4, 4, Piece()); // Clear path
    board.setPiece(5, 4, Piece()); // Clear path
    board.setPiece(6, 4, Piece()); // Remove white pawn
    
    // Place white queen that attacks black king
    board.setPiece(4, 4, Piece(PieceType::QUEEN, Color::WHITE));
    
    TestFramework::assert_true(board.isInCheck(Color::BLACK), "Black king in check from queen");
    TestFramework::assert_true(!board.isInCheck(Color::WHITE), "White king still not in check");
}

void test_legal_moves_generation() {
    Board board;
    
    // Get all legal moves for white at start
    auto whiteMoves = board.getAllLegalMoves(Color::WHITE);
    TestFramework::assert_equal(20, static_cast<int>(whiteMoves.size()), "White has 20 legal moves at start");
    
    // Get possible moves for a specific pawn
    auto pawnMoves = board.getPossibleMoves(6, 4); // e2 pawn
    TestFramework::assert_equal(2, static_cast<int>(pawnMoves.size()), "Pawn has 2 possible moves initially");
    
    // Get possible moves for a knight
    auto knightMoves = board.getPossibleMoves(7, 1); // b1 knight
    TestFramework::assert_equal(2, static_cast<int>(knightMoves.size()), "Knight has 2 possible moves initially");
}

void test_board_reset() {
    Board board;
    
    // Make some moves to change the board
    board.makeMove(Move(6, 4, 4, 4)); // e2-e4
    board.makeMove(Move(1, 4, 3, 4)); // e7-e5
    
    // Reset board
    board.resetToStartingPosition();
    
    // Check that board is back to starting position
    TestFramework::assert_equal(static_cast<int>(PieceType::PAWN), static_cast<int>(board.getPiece(6, 4).getType()), "Pawn back at e2");
    TestFramework::assert_equal(static_cast<int>(PieceType::PAWN), static_cast<int>(board.getPiece(1, 4).getType()), "Pawn back at e7");
    TestFramework::assert_true(board.getPiece(4, 4).isEmpty(), "e4 is empty again");
    TestFramework::assert_true(board.getPiece(3, 4).isEmpty(), "e5 is empty again");
    TestFramework::assert_equal(static_cast<int>(Color::WHITE), static_cast<int>(board.getGameState().currentPlayer), "White to move again");
}

void test_game_state_structure() {
    GameState state;
    
    // Test default GameState values
    TestFramework::assert_equal(static_cast<int>(Color::WHITE), static_cast<int>(state.currentPlayer), "Default current player is WHITE");
    TestFramework::assert_true(state.whiteCanCastleKingside, "Default white can castle kingside");
    TestFramework::assert_true(state.whiteCanCastleQueenside, "Default white can castle queenside");
    TestFramework::assert_true(state.blackCanCastleKingside, "Default black can castle kingside");
    TestFramework::assert_true(state.blackCanCastleQueenside, "Default black can castle queenside");
    TestFramework::assert_equal(-1, state.enPassantCol, "Default no en passant");
    TestFramework::assert_equal(0, state.halfMoveClock, "Default half move clock is 0");
    TestFramework::assert_equal(1, state.fullMoveNumber, "Default full move number is 1");
}

// Main function for standalone execution
int main() {
    std::cout << "Running Board Tests" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    TestFramework::run_test("Board Initialization", test_board_initialization);
    TestFramework::run_test("Board Utilities", test_board_utilities);
    TestFramework::run_test("Basic Move Validation", test_basic_move_validation);
    TestFramework::run_test("Move Execution", test_move_execution);
    TestFramework::run_test("Piece Setting", test_piece_setting);
    TestFramework::run_test("Check Detection", test_check_detection);
    TestFramework::run_test("Legal Moves Generation", test_legal_moves_generation);
    TestFramework::run_test("Board Reset", test_board_reset);
    TestFramework::run_test("GameState Structure", test_game_state_structure);
    
    TestFramework::print_summary();
    
    return TestFramework::all_tests_passed() ? 0 : 1;
}
