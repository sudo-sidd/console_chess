#include "../include/Board.h"
#include "../include/Utils.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// Constructor - initialize board to starting position
Board::Board() {
    // Initialize 8x8 board with empty pieces
    board.resize(8, std::vector<Piece>(8, Piece()));
    resetToStartingPosition();
}

// Reset board to standard chess starting position
void Board::resetToStartingPosition() {
    // Clear board
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            board[row][col] = Piece();
        }
    }
    
    // Place white pieces (bottom of board, rows 6-7)
    board[7][0] = Piece(PieceType::ROOK, Color::WHITE);
    board[7][1] = Piece(PieceType::KNIGHT, Color::WHITE);
    board[7][2] = Piece(PieceType::BISHOP, Color::WHITE);
    board[7][3] = Piece(PieceType::QUEEN, Color::WHITE);
    board[7][4] = Piece(PieceType::KING, Color::WHITE);
    board[7][5] = Piece(PieceType::BISHOP, Color::WHITE);
    board[7][6] = Piece(PieceType::KNIGHT, Color::WHITE);
    board[7][7] = Piece(PieceType::ROOK, Color::WHITE);
    
    // White pawns
    for (int col = 0; col < 8; ++col) {
        board[6][col] = Piece(PieceType::PAWN, Color::WHITE);
    }
    
    // Place black pieces (top of board, rows 0-1)
    board[0][0] = Piece(PieceType::ROOK, Color::BLACK);
    board[0][1] = Piece(PieceType::KNIGHT, Color::BLACK);
    board[0][2] = Piece(PieceType::BISHOP, Color::BLACK);
    board[0][3] = Piece(PieceType::QUEEN, Color::BLACK);
    board[0][4] = Piece(PieceType::KING, Color::BLACK);
    board[0][5] = Piece(PieceType::BISHOP, Color::BLACK);
    board[0][6] = Piece(PieceType::KNIGHT, Color::BLACK);
    board[0][7] = Piece(PieceType::ROOK, Color::BLACK);
    
    // Black pawns
    for (int col = 0; col < 8; ++col) {
        board[1][col] = Piece(PieceType::PAWN, Color::BLACK);
    }
    
    // Reset game state
    gameState = GameState();
}

// Board access methods
const Piece& Board::getPiece(int row, int col) const {
    if (!isOnBoard(row, col)) {
        static Piece empty;  // Return reference to empty piece for out-of-bounds
        return empty;
    }
    return board[row][col];
}

void Board::setPiece(int row, int col, const Piece& piece) {
    if (isOnBoard(row, col)) {
        board[row][col] = piece;
    }
}

// Check if coordinates are on the board
bool Board::isOnBoard(int row, int col) const {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

// Check if path is clear for sliding pieces (rook, bishop, queen)
bool Board::isPathClear(int fromRow, int fromCol, int toRow, int toCol) const {
    int deltaRow = toRow - fromRow;
    int deltaCol = toCol - fromCol;
    
    // Determine step direction
    int stepRow = (deltaRow == 0) ? 0 : (deltaRow > 0 ? 1 : -1);
    int stepCol = (deltaCol == 0) ? 0 : (deltaCol > 0 ? 1 : -1);
    
    int currentRow = fromRow + stepRow;
    int currentCol = fromCol + stepCol;
    
    // Check each square in the path (excluding destination)
    while (currentRow != toRow || currentCol != toCol) {
        if (!board[currentRow][currentCol].isEmpty()) {
            return false;  // Path is blocked
        }
        currentRow += stepRow;
        currentCol += stepCol;
    }
    
    return true;
}

// Find king position for a given color
void Board::findKing(Color color, int& kingRow, int& kingCol) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const Piece& piece = board[row][col];
            if (piece.getType() == PieceType::KING && piece.getColor() == color) {
                kingRow = row;
                kingCol = col;
                return;
            }
        }
    }
    // This should never happen in a valid game
    kingRow = -1;
    kingCol = -1;
}

// Check if a color is in check
bool Board::isInCheck(Color color) const {
    int kingRow, kingCol;
    findKing(color, kingRow, kingCol);
    
    if (kingRow == -1) return false;  // No king found (shouldn't happen)
    
    // Check if any enemy piece can attack the king
    Color enemyColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const Piece& piece = board[row][col];
            if (piece.getColor() == enemyColor && !piece.isEmpty()) {
                Move attackMove(row, col, kingRow, kingCol);
                if (isValidMove(attackMove)) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

// Validate if a move is legal
bool Board::isValidMove(const Move& move) const {
    if (!isOnBoard(move.fromRow, move.fromCol) || !isOnBoard(move.toRow, move.toCol)) {
        return false;
    }
    
    const Piece& fromPiece = board[move.fromRow][move.fromCol];
    const Piece& toPiece = board[move.toRow][move.toCol];
    
    // Must have a piece to move
    if (fromPiece.isEmpty()) return false;
    
    // Must be moving your own piece
    if (fromPiece.getColor() != gameState.currentPlayer) return false;
    
    // Can't capture own piece
    if (!toPiece.isEmpty() && fromPiece.getColor() == toPiece.getColor()) {
        return false;
    }
    
    // Check basic piece movement rules
    if (!fromPiece.canMoveTo(move.fromRow, move.fromCol, move.toRow, move.toCol)) {
        return false;
    }
    
    // Check path is clear for sliding pieces
    if (fromPiece.getType() == PieceType::ROOK || 
        fromPiece.getType() == PieceType::BISHOP || 
        fromPiece.getType() == PieceType::QUEEN) {
        if (!isPathClear(move.fromRow, move.fromCol, move.toRow, move.toCol)) {
            return false;
        }
    }
    
    // Special pawn rules
    if (fromPiece.getType() == PieceType::PAWN) {
        int deltaCol = move.toCol - move.fromCol;
        
        // Forward move - destination must be empty
        if (deltaCol == 0 && !toPiece.isEmpty()) {
            return false;
        }
        
        // Diagonal move - must be capturing or en passant
        if (deltaCol != 0) {
            if (toPiece.isEmpty()) {
                // Check for en passant
                if (gameState.enPassantCol != move.toCol) return false;
                int expectedRow = (fromPiece.getColor() == Color::WHITE) ? 3 : 4;
                if (move.fromRow != expectedRow) return false;
            }
        }
    }
    
    // Check if move would leave king in check (simulate the move)
    if (wouldBeInCheck(fromPiece.getColor(), move)) {
        return false;
    }
    
    return true;
}

// Check if making a move would leave the king in check
bool Board::wouldBeInCheck(Color color, const Move& move) const {
    // Make a copy of the board to test the move
    Board testBoard = *this;
    
    // Make the move on the test board
    testBoard.board[move.toRow][move.toCol] = testBoard.board[move.fromRow][move.fromCol];
    testBoard.board[move.fromRow][move.fromCol] = Piece();
    
    // Check if king is in check after the move
    return testBoard.isInCheck(color);
}

// Convert board position to algebraic notation
std::string Board::getSquareName(int row, int col) const {
    if (!isOnBoard(row, col)) return "??";
    
    std::string result;
    result += ChessUtils::colToFile(col);
    result += ChessUtils::rowToRank(row);
    return result;
}

// Display the board
void Board::display() const {
    using namespace ChessUtils;
    
    // Calculate centering offset (assuming terminal width of 80 characters)
    const int boardWidth = 37;  // Adjusted for simpler layout
    const int terminalWidth = 80;
    const int centerOffset = (terminalWidth - boardWidth) / 2;
    std::string indent(centerOffset, ' ');
    
    std::cout << "\n";
    
    // Top border with file labels
    std::cout << indent << "   ";
    for (char file = 'a'; file <= 'h'; ++file) {
        std::cout << " " << file << "  ";
    }
    std::cout << "\n";
    
    std::cout << indent << "  ┌";
    for (int i = 0; i < 8; ++i) {
        std::cout << "───";
        if (i < 7) std::cout << "┬";
    }
    std::cout << "┐\n";
    
    for (int row = 0; row < 8; ++row) {
        // Rank number on the left
        std::cout << indent << ChessUtils::rowToRank(row) << " │";
        
        for (int col = 0; col < 8; ++col) {
            // Alternate square colors
            bool isLightSquare = (row + col) % 2 == 0;
            
            // Set square background color
            if (isLightSquare) {
                std::cout << "\033[47m\033[30m";  // White background, black text
            } else {
                std::cout << "\033[46m\033[30m";  // Cyan background, black text
            }
            
            // Print piece with padding
            std::cout << " " << board[row][col].getSymbol() << " ";
            
            // Reset colors before border
            std::cout << "\033[0m";
            
            std::cout << "│";
        }
        
        // Rank number on the right
        std::cout << " " << ChessUtils::rowToRank(row) << "\n";
        
        // Middle borders (except after last row)
        if (row < 7) {
            std::cout << indent << "  ├";
            for (int i = 0; i < 8; ++i) {
                std::cout << "───";
                if (i < 7) std::cout << "┼";
            }
            std::cout << "┤\n";
        }
    }
    
    // Bottom border
    std::cout << indent << "  └";
    for (int i = 0; i < 8; ++i) {
        std::cout << "───";
        if (i < 7) std::cout << "┴";
    }
    std::cout << "┘\n";
    
    // Bottom file labels
    std::cout << indent << "   ";
    for (char file = 'a'; file <= 'h'; ++file) {
        std::cout << " " << file << "  ";
    }
    std::cout << "\n\n";
    
    // Display current player (centered)
    std::string playerText = "Current player: ";
    playerText += (gameState.currentPlayer == Color::WHITE ? "White" : "Black");
    int playerOffset = (terminalWidth - playerText.length()) / 2;
    std::cout << std::string(playerOffset, ' ') << playerText << std::endl;
}

// Make a move on the board
bool Board::makeMove(const Move& move) {
    if (!isValidMove(move)) return false;
    
    const Piece& movingPiece = board[move.fromRow][move.fromCol];
    const Piece& capturedPiece = board[move.toRow][move.toCol];
    
    // Handle en passant capture
    if (movingPiece.getType() == PieceType::PAWN && 
        move.fromCol != move.toCol && capturedPiece.isEmpty()) {
        // En passant capture - remove the captured pawn
        int capturedPawnRow = (movingPiece.getColor() == Color::WHITE) ? move.toRow + 1 : move.toRow - 1;
        board[capturedPawnRow][move.toCol] = Piece();
    }
    
    // Make the move
    board[move.toRow][move.toCol] = board[move.fromRow][move.fromCol];
    board[move.fromRow][move.fromCol] = Piece();
    
    // Handle pawn promotion
    if (movingPiece.getType() == PieceType::PAWN) {
        if ((movingPiece.getColor() == Color::WHITE && move.toRow == 0) ||
            (movingPiece.getColor() == Color::BLACK && move.toRow == 7)) {
            // Promote to queen by default (can be extended for user choice)
            board[move.toRow][move.toCol] = Piece(PieceType::QUEEN, movingPiece.getColor());
        }
    }
    
    // Handle castling
    if (movingPiece.getType() == PieceType::KING && abs(move.toCol - move.fromCol) == 2) {
        // Castling - move the rook too
        if (move.toCol > move.fromCol) {  // Kingside castling
            board[move.toRow][5] = board[move.toRow][7];  // Move rook to f-file
            board[move.toRow][7] = Piece();  // Clear rook's original position
        } else {  // Queenside castling
            board[move.toRow][3] = board[move.toRow][0];  // Move rook to d-file
            board[move.toRow][0] = Piece();  // Clear rook's original position
        }
    }
    
    // Update game state
    updateGameState(move, movingPiece, capturedPiece);
    
    // Switch turns
    gameState.currentPlayer = (gameState.currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
    
    return true;
}

// Update game state after a move (helper method)
void Board::updateGameState(const Move& move, const Piece& movingPiece, const Piece& capturedPiece) {
    // Update castling rights
    if (movingPiece.getType() == PieceType::KING) {
        if (movingPiece.getColor() == Color::WHITE) {
            gameState.whiteCanCastleKingside = false;
            gameState.whiteCanCastleQueenside = false;
        } else {
            gameState.blackCanCastleKingside = false;
            gameState.blackCanCastleQueenside = false;
        }
    }
    
    if (movingPiece.getType() == PieceType::ROOK) {
        if (movingPiece.getColor() == Color::WHITE) {
            if (move.fromRow == 7 && move.fromCol == 0) gameState.whiteCanCastleQueenside = false;
            if (move.fromRow == 7 && move.fromCol == 7) gameState.whiteCanCastleKingside = false;
        } else {
            if (move.fromRow == 0 && move.fromCol == 0) gameState.blackCanCastleQueenside = false;
            if (move.fromRow == 0 && move.fromCol == 7) gameState.blackCanCastleKingside = false;
        }
    }
    
    // Update en passant
    gameState.enPassantCol = -1;  // Reset en passant
    if (movingPiece.getType() == PieceType::PAWN && abs(move.toRow - move.fromRow) == 2) {
        gameState.enPassantCol = move.fromCol;  // Pawn moved two squares
    }
    
    // Update move counters
    if (movingPiece.getType() == PieceType::PAWN || !capturedPiece.isEmpty()) {
        gameState.halfMoveClock = 0;  // Reset on pawn move or capture
    } else {
        gameState.halfMoveClock++;
    }
    
    if (gameState.currentPlayer == Color::BLACK) {
        gameState.fullMoveNumber++;
    }
}

// Get all legal moves for a color
std::vector<Move> Board::getAllLegalMoves(Color color) const {
    std::vector<Move> legalMoves;
    
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const Piece& piece = board[row][col];
            if (piece.getColor() == color && !piece.isEmpty()) {
                std::vector<Move> pieceMoves = getPossibleMoves(row, col);
                legalMoves.insert(legalMoves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }
    
    return legalMoves;
}

// Get possible moves for a piece at given position
std::vector<Move> Board::getPossibleMoves(int row, int col) const {
    std::vector<Move> moves;
    const Piece& piece = board[row][col];
    
    if (piece.isEmpty()) return moves;
    
    // Generate all possible destination squares
    for (int toRow = 0; toRow < 8; ++toRow) {
        for (int toCol = 0; toCol < 8; ++toCol) {
            Move move(row, col, toRow, toCol);
            if (isValidMove(move)) {
                moves.push_back(move);
            }
        }
    }
    
    return moves;
}

// Check for checkmate
bool Board::isCheckmate(Color color) const {
    if (!isInCheck(color)) return false;  // Not in check, so not checkmate
    
    // If in check, see if there are any legal moves
    std::vector<Move> legalMoves = getAllLegalMoves(color);
    return legalMoves.empty();
}

// Check for stalemate
bool Board::isStalemate(Color color) const {
    if (isInCheck(color)) return false;  // In check, so not stalemate
    
    // If not in check, see if there are any legal moves
    std::vector<Move> legalMoves = getAllLegalMoves(color);
    return legalMoves.empty();
}

// Check for draw conditions
bool Board::isDraw() const {
    // 50-move rule
    if (gameState.halfMoveClock >= 50) return true;
    
    // TODO: Add insufficient material check
    // TODO: Add threefold repetition check
    
    return false;
}

// Evaluate position for AI (positive = white advantage)
int Board::evaluatePosition() const {
    int score = 0;
    
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const Piece& piece = board[row][col];
            if (!piece.isEmpty()) {
                int pieceValue = piece.getValue();
                if (piece.getColor() == Color::BLACK) {
                    pieceValue = -pieceValue;
                }
                score += pieceValue;
            }
        }
    }
    
    return score;
}
