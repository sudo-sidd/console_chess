# Makefile for Console Chess Game

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I.
DEBUGFLAGS = -std=c++17 -Wall -Wextra -g -I.

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj

# Files
SOURCES = $(wildcard $(SRCDIR)/*.cpp) main.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)
TARGET = chess_game

# Default target
all: $(TARGET)

# Create object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/$(SRCDIR)

# Build the executable
$(TARGET): $(OBJDIR) $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compile source files
$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug build
debug: CXXFLAGS = $(DEBUGFLAGS)
debug: $(TARGET)

# Clean build files
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Run the game
run: $(TARGET)
	./$(TARGET)

# Install dependencies (if needed)
install-deps:
	@echo "No external dependencies required for this project."
	@echo "Make sure you have a C++17 compatible compiler (g++ 7+ or clang++ 5+)"

# Help
help:
	@echo "Available targets:"
	@echo "  all       - Build the chess game (default)"
	@echo "  debug     - Build with debug symbols"
	@echo "  clean     - Remove build files"
	@echo "  run       - Build and run the game"
	@echo "  help      - Show this help message"

# Phony targets
.PHONY: all debug clean run install-deps help
