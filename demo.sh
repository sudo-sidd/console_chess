#!/bin/bash

# Demo script for Console Chess Game
# This script demonstrates the chess game with some automated moves

echo "=== Console Chess Game Demo ==="
echo "This demo will show the improved board display and a few sample moves."
echo

# Compile the game
echo "Building the chess game..."
make clean && make

if [ $? -ne 0 ]; then
    echo "Build failed! Please check for compilation errors."
    exit 1
fi

echo
echo "✅ Build successful!"
echo
echo "Demo: Improved Board Display"
echo "============================"
echo
echo "Key improvements:"
echo "✓ Horizontally centered board"
echo "✓ Larger, more readable squares"  
echo "✓ Properly aligned coordinates (a-h, 1-8)"
echo "✓ Beautiful Unicode chess pieces"
echo "✓ Clean box-drawing borders"
echo
echo "The board is now much more visually appealing and easier to read!"
echo
echo "Starting the chess game..."
echo "You can now play with proper move notation like 'e2 e4', 'g1 f3', etc."
echo
echo "Press Enter to start the game, or Ctrl+C to exit the demo."
read

# Start the game
./chess_game
