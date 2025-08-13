# Chess Game Testing Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I../include
OBJDIR = ../obj
SRCDIR = ../src

# Source files needed for tests
UTILS_OBJ = $(OBJDIR)/Utils.o
PIECE_OBJ = $(OBJDIR)/Piece.o  
BOARD_OBJ = $(OBJDIR)/Board.o
AI_OBJ = $(OBJDIR)/AI.o

# Test executables
TEST_UTILS = test_utils
TEST_PIECE = test_piece
TEST_BOARD = test_board
TEST_ALL = test_all

.PHONY: all tests clean run-tests help

all: tests

# Build required object files first
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Individual test targets
$(TEST_UTILS): test_utils.cpp $(UTILS_OBJ)
	$(CXX) $(CXXFLAGS) test_utils.cpp $(UTILS_OBJ) -o $(TEST_UTILS)

$(TEST_PIECE): test_piece.cpp $(PIECE_OBJ) $(UTILS_OBJ)
	$(CXX) $(CXXFLAGS) test_piece.cpp $(PIECE_OBJ) $(UTILS_OBJ) -o $(TEST_PIECE)

$(TEST_BOARD): test_board.cpp $(BOARD_OBJ) $(PIECE_OBJ) $(UTILS_OBJ)
	$(CXX) $(CXXFLAGS) test_board.cpp $(BOARD_OBJ) $(PIECE_OBJ) $(UTILS_OBJ) -o $(TEST_BOARD)

# Combined test runner (optional - simpler to run individual tests)
$(TEST_ALL): $(UTILS_OBJ) $(PIECE_OBJ) $(BOARD_OBJ)
	@echo "Building comprehensive test suite..."
	$(CXX) $(CXXFLAGS) -DTEST_UTILS_FUNCS test_utils.cpp $(UTILS_OBJ) -c -o test_utils_funcs.o
	$(CXX) $(CXXFLAGS) -DTEST_PIECE_FUNCS test_piece.cpp $(PIECE_OBJ) $(UTILS_OBJ) -c -o test_piece_funcs.o  
	$(CXX) $(CXXFLAGS) -DTEST_BOARD_FUNCS test_board.cpp $(BOARD_OBJ) $(PIECE_OBJ) $(UTILS_OBJ) -c -o test_board_funcs.o
	$(CXX) $(CXXFLAGS) test_runner.cpp test_utils_funcs.o test_piece_funcs.o test_board_funcs.o $(UTILS_OBJ) $(PIECE_OBJ) $(BOARD_OBJ) -o $(TEST_ALL)

# Build all tests
tests: $(TEST_UTILS) $(TEST_PIECE) $(TEST_BOARD)

# Run all tests
run-tests: tests
	@echo "Running Utils Tests..."
	@./$(TEST_UTILS)
	@echo ""
	@echo "Running Piece Tests..."  
	@./$(TEST_PIECE)
	@echo ""
	@echo "Running Board Tests..."
	@./$(TEST_BOARD)
	@echo ""
	@echo "All tests completed!"

# Run individual test suites
run-utils: $(TEST_UTILS)
	./$(TEST_UTILS)

run-piece: $(TEST_PIECE)
	./$(TEST_PIECE)

run-board: $(TEST_BOARD)
	./$(TEST_BOARD)

# Clean test files
clean:
	rm -f $(TEST_UTILS) $(TEST_PIECE) $(TEST_BOARD) $(TEST_ALL)
	rm -f *.o

# Help target
help:
	@echo "Chess Game Test Makefile"
	@echo "========================"
	@echo "Available targets:"
	@echo "  tests      - Build all test executables"
	@echo "  run-tests  - Build and run all tests"
	@echo "  run-utils  - Run utility function tests"
	@echo "  run-piece  - Run piece class tests"
	@echo "  run-board  - Run board class tests"
	@echo "  clean      - Remove test executables"
	@echo "  help       - Show this help message"
