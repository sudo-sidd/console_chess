#include "../include/Utils.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace ChessUtils {

// String manipulation utilities
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    
    return tokens;
}

std::string toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Chess notation utilities
bool parseMove(const std::string& input, int& fromRow, int& fromCol, int& toRow, int& toCol) {
    std::string trimmed = trim(input);
    std::vector<std::string> parts = split(trimmed, ' ');
    
    if (parts.size() != 2) return false;
    
    // Parse "from" square (e.g., "e2")
    if (parts[0].length() != 2) return false;
    char fromFile = std::tolower(parts[0][0]);
    char fromRank = parts[0][1];
    
    if (fromFile < 'a' || fromFile > 'h' || fromRank < '1' || fromRank > '8') {
        return false;
    }
    
    // Parse "to" square (e.g., "e4")
    if (parts[1].length() != 2) return false;
    char toFile = std::tolower(parts[1][0]);
    char toRank = parts[1][1];
    
    if (toFile < 'a' || toFile > 'h' || toRank < '1' || toRank > '8') {
        return false;
    }
    
    // Convert to array indices (0-7)
    fromCol = fileToCol(fromFile);
    fromRow = rankToRow(fromRank);
    toCol = fileToCol(toFile);
    toRow = rankToRow(toRank);
    
    return true;
}

std::string moveToString(int fromRow, int fromCol, int toRow, int toCol) {
    std::string result;
    result += colToFile(fromCol);
    result += rowToRank(fromRow);
    result += " ";
    result += colToFile(toCol);
    result += rowToRank(toRow);
    return result;
}

// Console utilities
void clearScreen() {
    // Cross-platform screen clearing
    #ifdef _WIN32
        system("cls");
    #else
        std::cout << "\033[2J\033[H";  // ANSI clear screen
    #endif
}

void setTextColor(int color) {
    std::cout << "\033[" << color << "m";
}

void resetTextColor() {
    std::cout << "\033[0m";
}

// Input utilities
std::string getInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    if (std::cin.rdbuf()->in_avail() > 0) {
        std::cin.ignore(10000, '\n');
    }
    std::getline(std::cin, input);
    return trim(input);
}

int getChoice(const std::string& prompt, int minChoice, int maxChoice) {
    int choice;
    while (true) {
        std::cout << prompt;
        if (std::cin >> choice && choice >= minChoice && choice <= maxChoice) {
            std::cin.ignore();  // Clear the newline character
            return choice;
        }
        
        // Clear error state and input buffer
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input. Please enter a number between " 
                  << minChoice << " and " << maxChoice << ".\n";
    }
}

// Chess coordinate conversion
int fileToCol(char file) {
    return file - 'a';  // 'a' = 0, 'b' = 1, etc.
}

int rankToRow(char rank) {
    return '8' - rank;  // '8' = 0, '7' = 1, ..., '1' = 7 (top to bottom)
}

char colToFile(int col) {
    return 'a' + col;  // 0 = 'a', 1 = 'b', etc.
}

char rowToRank(int row) {
    return '8' - row;  // 0 = '8', 1 = '7', ..., 7 = '1'
}

} // namespace ChessUtils
