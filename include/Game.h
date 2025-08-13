#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "AI.h"
#include <memory>  // For smart pointers - modern C++

// Game mode enumeration
enum class GameMode {
    PLAYER_VS_PLAYER,
    PLAYER_VS_AI_WHITE,  // Player plays as white
    PLAYER_VS_AI_BLACK   // Player plays as black
};

class Game {
private:
    Board board;
    GameMode currentMode;
    std::unique_ptr<AI> ai;  // Smart pointer - automatically manages memory
    bool gameRunning;
    
    // Game loop methods
    void showMainMenu();
    void selectGameMode();
    void playPlayerVsPlayer();
    void playPlayerVsAI();
    
    // Input and move handling
    Move getPlayerMove();
    bool parsePlayerInput(const std::string& input, Move& move);
    void handleSpecialMoves(Move& move);
    
    // Game state display
    void displayGameState();
    void displayMoveHistory();
    void showGameResult();
    
    // Utility methods
    void waitForEnter();
    void showInstructions();
    void showAbout();
    
    // Move validation and execution
    bool executeMove(const Move& move);
    void undoLastMove();  // For future implementation
    
    // Game ending conditions
    bool checkGameEnd();
    void handleGameEnd();

public:
    // Constructor and destructor
    Game();
    ~Game() = default;  // Compiler-generated destructor is fine
    
    // Main game loop
    void run();
    
    // Game control
    void startNewGame();
    void quitGame();
    
    // Settings
    void changeAIDifficulty();
    void toggleDisplaySettings();
    
    // Game state queries
    bool isGameRunning() const { return gameRunning; }
    GameMode getCurrentMode() const { return currentMode; }
    
    // Static utility methods
    void showWelcomeMessage();
    void showGoodbyeMessage();
};

#endif // GAME_H
