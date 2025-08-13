#include "../include/Game.h"
#include "../include/Utils.h"
#include <iostream>
#include <memory>
#include <limits>

// Constructor
Game::Game() : currentMode(GameMode::PLAYER_VS_PLAYER), gameRunning(false) {
    // AI will be created when needed
}

// Main game loop
void Game::run() {
    showWelcomeMessage();
    gameRunning = true;
    
    while (gameRunning) {
        showMainMenu();
    }
    
    showGoodbyeMessage();
}

// Show welcome message
void Game::showWelcomeMessage() {
    ChessUtils::clearScreen();
    std::cout << "╔══════════════════════════════════════════════╗\n";
    std::cout << "║              CONSOLE CHESS GAME              ║\n";
    std::cout << "║                                              ║\n";
    std::cout << "║     A full-featured chess game in C++        ║\n";
    std::cout << "║                                              ║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n\n";
    
    std::cout << "Features:\n";
    std::cout << "• Player vs Player mode\n";
    std::cout << "• AI opponent with multiple difficulty levels\n";
    std::cout << "• Complete chess rules implementation\n";
    std::cout << "• Beautiful Unicode display\n\n";
    
    std::cout << "Press Enter to continue...";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Show goodbye message
void Game::showGoodbyeMessage() {
    ChessUtils::clearScreen();
    std::cout << "Thanks for playing Console Chess!\n";
    std::cout << "Hope you enjoyed the game. Goodbye!\n\n";
}

// Main menu
void Game::showMainMenu() {
    ChessUtils::clearScreen();
    std::cout << "=== CHESS GAME MAIN MENU ===\n\n";
    std::cout << "1. Player vs Player\n";
    std::cout << "2. Player vs AI (You play White)\n";
    std::cout << "3. Player vs AI (You play Black)\n";
    std::cout << "4. Instructions\n";
    std::cout << "5. About\n";
    std::cout << "6. Quit\n\n";
    
    int choice = ChessUtils::getChoice("Enter your choice (1-6): ", 1, 6);
    
    switch (choice) {
        case 1:
            currentMode = GameMode::PLAYER_VS_PLAYER;
            startNewGame();
            break;
        case 2:
            currentMode = GameMode::PLAYER_VS_AI_WHITE;
            ai = std::make_unique<AI>(AILevel::MEDIUM, Color::BLACK);
            startNewGame();
            break;
        case 3:
            currentMode = GameMode::PLAYER_VS_AI_BLACK;
            ai = std::make_unique<AI>(AILevel::MEDIUM, Color::WHITE);
            startNewGame();
            break;
        case 4:
            showInstructions();
            break;
        case 5:
            showAbout();
            break;
        case 6:
            quitGame();
            break;
    }
}

// Start a new game
void Game::startNewGame() {
    board.resetToStartingPosition();
    
    if (currentMode == GameMode::PLAYER_VS_PLAYER) {
        playPlayerVsPlayer();
    } else {
        playPlayerVsAI();
    }
}

// Player vs Player game loop
void Game::playPlayerVsPlayer() {
    while (!checkGameEnd()) {
        displayGameState();
        
        Move playerMove = getPlayerMove();
        
        // Check for quit move
        if (playerMove.fromRow == -1) {
            return;  // Player wants to quit
        }
        
        if (executeMove(playerMove)) {
            // Move was successful, continue
        } else {
            std::cout << "Invalid move! Please try again.\n";
            waitForEnter();
        }
    }
    
    handleGameEnd();
}

// Player vs AI game loop
void Game::playPlayerVsAI() {
    while (!checkGameEnd()) {
        displayGameState();
        
        Color currentPlayer = board.getGameState().currentPlayer;
        
        if (currentPlayer != ai->getColor()) {
            // Human player's turn
            Move playerMove = getPlayerMove();
            
            // Check for quit move
            if (playerMove.fromRow == -1) {
                return;  // Player wants to quit
            }
            
            if (!executeMove(playerMove)) {
                std::cout << "Invalid move! Please try again.\n";
                waitForEnter();
                continue;
            }
        } else {
            // AI's turn
            std::cout << "AI is thinking...\n";
            Move aiMove = ai->getBestMove(board);
            
            if (executeMove(aiMove)) {
                std::cout << "AI moves: " << ChessUtils::moveToString(
                    aiMove.fromRow, aiMove.fromCol, aiMove.toRow, aiMove.toCol) << "\n";
                waitForEnter();
            } else {
                std::cout << "AI made an invalid move! This shouldn't happen.\n";
                waitForEnter();
                break;
            }
        }
    }
    
    handleGameEnd();
}

// Get player move input
Move Game::getPlayerMove() {
    while (true) {
        std::string input = ChessUtils::getInput("Enter your move (e.g., 'e2 e4') or 'quit' to return to menu: ");
        
        if (ChessUtils::toLowerCase(input) == "quit") {
            return Move(-1, -1, -1, -1);  // Special move to indicate quit
        }
        
        Move move(0, 0, 0, 0);
        if (parsePlayerInput(input, move)) {
            return move;
        } else {
            std::cout << "Invalid input format. Please use format like 'e2 e4'.\n";
        }
    }
}

// Parse player input into a move
bool Game::parsePlayerInput(const std::string& input, Move& move) {
    int fromRow, fromCol, toRow, toCol;
    if (ChessUtils::parseMove(input, fromRow, fromCol, toRow, toCol)) {
        move = Move(fromRow, fromCol, toRow, toCol);
        return true;
    }
    return false;
}

// Execute a move
bool Game::executeMove(const Move& move) {
    // Check for quit move
    if (move.fromRow == -1) {
        return false;  // Player wants to quit
    }
    
    return board.makeMove(move);
}

// Display current game state
void Game::displayGameState() {
    ChessUtils::clearScreen();
    
    // Show game mode
    std::cout << "Mode: ";
    switch (currentMode) {
        case GameMode::PLAYER_VS_PLAYER:
            std::cout << "Player vs Player";
            break;
        case GameMode::PLAYER_VS_AI_WHITE:
            std::cout << "Player (White) vs AI (Black)";
            break;
        case GameMode::PLAYER_VS_AI_BLACK:
            std::cout << "Player (Black) vs AI (White)";
            break;
    }
    std::cout << "\n\n";
    
    // Display the board
    board.display();
    
    // Show game status
    Color currentPlayer = board.getGameState().currentPlayer;
    if (board.isInCheck(currentPlayer)) {
        std::cout << "\n*** CHECK! ***\n";
    }
    
    std::cout << "\n";
}

// Check if game has ended
bool Game::checkGameEnd() {
    Color currentPlayer = board.getGameState().currentPlayer;
    
    if (board.isCheckmate(currentPlayer)) {
        return true;
    }
    
    if (board.isStalemate(currentPlayer)) {
        return true;
    }
    
    if (board.isDraw()) {
        return true;
    }
    
    return false;
}

// Handle game end
void Game::handleGameEnd() {
    displayGameState();
    
    Color currentPlayer = board.getGameState().currentPlayer;
    
    if (board.isCheckmate(currentPlayer)) {
        Color winner = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
        std::cout << "CHECKMATE! " << (winner == Color::WHITE ? "White" : "Black") << " wins!\n";
    } else if (board.isStalemate(currentPlayer)) {
        std::cout << "STALEMATE! The game is a draw.\n";
    } else if (board.isDraw()) {
        std::cout << "DRAW! The game ends in a draw.\n";
    }
    
    std::cout << "\nGame Over!\n";
    waitForEnter();
}

// Show instructions
void Game::showInstructions() {
    ChessUtils::clearScreen();
    std::cout << "=== HOW TO PLAY ===\n\n";
    
    std::cout << "MOVING PIECES:\n";
    std::cout << "• Enter moves in algebraic notation: 'e2 e4'\n";
    std::cout << "• First coordinate is the source square\n";
    std::cout << "• Second coordinate is the destination square\n";
    std::cout << "• Examples: 'e2 e4', 'g1 f3', 'e1 g1' (castling)\n\n";
    
    std::cout << "SPECIAL MOVES:\n";
    std::cout << "• Castling: Move king two squares toward rook\n";
    std::cout << "• En passant: Automatic when legal\n";
    std::cout << "• Pawn promotion: Pawns automatically promote to queens\n\n";
    
    std::cout << "GAME MODES:\n";
    std::cout << "• Player vs Player: Take turns on same computer\n";
    std::cout << "• Player vs AI: Choose your color and play against AI\n\n";
    
    std::cout << "CONTROLS:\n";
    std::cout << "• Type 'quit' during move input to return to main menu\n";
    std::cout << "• Press Enter to continue after messages\n\n";
    
    waitForEnter();
}

// Show about information
void Game::showAbout() {
    ChessUtils::clearScreen();
    std::cout << "=== ABOUT CONSOLE CHESS ===\n\n";
    
    std::cout << "This is a full-featured chess game written in C++.\n\n";
    
    std::cout << "FEATURES:\n";
    std::cout << "• Complete chess rules implementation\n";
    std::cout << "• AI opponent with minimax algorithm\n";
    std::cout << "• Beautiful Unicode chess pieces\n";
    std::cout << "• Colored board display\n";
    std::cout << "• Support for all special moves\n\n";
    
    std::cout << "LEARNING C++:\n";
    std::cout << "This project demonstrates many C++ concepts:\n";
    std::cout << "• Classes and object-oriented programming\n";
    std::cout << "• STL containers (vectors, strings)\n";
    std::cout << "• Enums and strong typing\n";
    std::cout << "• Smart pointers and memory management\n";
    std::cout << "• Algorithms and game AI\n\n";
    
    std::cout << "Enjoy learning C++ through chess!\n\n";
    
    waitForEnter();
}

// Wait for user to press Enter
void Game::waitForEnter() {
    std::cout << "Press Enter to continue...";
    std::cout.flush();
    
    if (std::cin.eof()) {
        return;  // Just return if EOF
    }
    
    // Clear any pending input and wait for enter
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Quit the game
void Game::quitGame() {
    gameRunning = false;
}
