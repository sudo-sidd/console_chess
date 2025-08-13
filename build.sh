#!/bin/bash

# Build script for Console Chess Game
# This script will compile and optionally run the chess game

echo "=== Console Chess Game Build Script ==="
echo

# Check if g++ is available
if ! command -v g++ &> /dev/null; then
    echo "Error: g++ compiler not found!"
    echo "Please install g++ (part of GCC) to compile this project."
    echo "On Ubuntu/Debian: sudo apt install g++"
    echo "On CentOS/RHEL: sudo yum install gcc-c++"
    echo "On macOS: install Xcode command line tools"
    exit 1
fi

# Check C++17 support
echo "Checking compiler version..."
g++ --version | head -n 1

# Clean previous builds
echo "Cleaning previous build files..."
make clean

# Compile the project
echo "Compiling the chess game..."
if make; then
    echo
    echo "✅ Compilation successful!"
    echo "The executable 'chess_game' has been created."
    echo
    
    # Ask if user wants to run the game
    read -p "Do you want to run the chess game now? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "Starting the chess game..."
        echo "=============================="
        ./chess_game
    else
        echo "You can run the game later with: ./chess_game"
    fi
else
    echo
    echo "❌ Compilation failed!"
    echo "Please check the error messages above."
    exit 1
fi
