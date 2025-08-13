#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

// Utility functions for the chess game

namespace ChessUtils {
    // String manipulation utilities
    std::string trim(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string toLowerCase(const std::string& str);
    
    // Chess notation utilities
    bool parseMove(const std::string& input, int& fromRow, int& fromCol, int& toRow, int& toCol);
    std::string moveToString(int fromRow, int fromCol, int toRow, int toCol);
    
    // Console utilities
    void clearScreen();
    void setTextColor(int color);
    void resetTextColor();
    
    // Input utilities
    std::string getInput(const std::string& prompt);
    int getChoice(const std::string& prompt, int minChoice, int maxChoice);
    
    // Chess coordinate conversion
    int fileToCol(char file);  // 'a' -> 0, 'b' -> 1, etc.
    int rankToRow(char rank);  // '1' -> 7, '2' -> 6, etc. (board is 0-indexed from top)
    char colToFile(int col);   // 0 -> 'a', 1 -> 'b', etc.
    char rowToRank(int row);   // 7 -> '1', 6 -> '2', etc.
}

// ANSI color codes for terminal output
namespace Colors {
    const int BLACK = 30;
    const int RED = 31;
    const int GREEN = 32;
    const int YELLOW = 33;
    const int BLUE = 34;
    const int MAGENTA = 35;
    const int CYAN = 36;
    const int WHITE = 37;
    const int RESET = 0;
    
    // Background colors
    const int BG_BLACK = 40;
    const int BG_RED = 41;
    const int BG_GREEN = 42;
    const int BG_YELLOW = 43;
    const int BG_BLUE = 44;
    const int BG_MAGENTA = 45;
    const int BG_CYAN = 46;
    const int BG_WHITE = 47;
}

#endif // UTILS_H
