# Chess Game Testing Framework

This directory contains comprehensive unit tests for the Console Chess Game project.

## Overview

The testing framework is built using a custom, lightweight testing framework (`test_framework.h`) that provides:
- Simple assertion methods (`assert_true`, `assert_equal`)
- Test organization and execution
- Clear test result reporting
- Exception handling for robust testing

## Test Structure

### Test Files

1. **`test_utils.cpp`** - Tests for utility functions
   - String manipulation (trim, toLowerCase, split)
   - Chess coordinate conversion (fileToCol, rankToRow, etc.)
   - Move parsing and string conversion
   - **46 tests total**

2. **`test_piece.cpp`** - Tests for piece functionality
   - Piece construction and properties
   - Piece values (using centipawn system)
   - Unicode symbol generation
   - Movement validation for all piece types
   - Move structure validation
   - **82 tests total**

3. **`test_board.cpp`** - Tests for board functionality
   - Board initialization and setup
   - Basic move validation and execution
   - Check detection and game state management
   - Legal move generation
   - Board utilities and coordinate system
   - **65 tests total**

### Test Framework Components

- **`test_framework.h`** - Core testing infrastructure
- **`Makefile`** - Test compilation and execution
- Individual test executables can be run standalone

## Running Tests

### From Main Project
```bash
# Run all tests
make test

# Run specific test suites
make test-utils
make test-piece
make test-board

# Clean test files
make test-clean
```

### From Tests Directory
```bash
cd tests/

# Run all tests
make run-tests

# Run individual test suites
make run-utils
make run-piece  
make run-board

# Build tests (without running)
make tests

# Clean
make clean

# Help
make help
```

## Test Results

**Current Status: ✅ ALL TESTS PASSING**
- **Total Tests: 193**
- **Utils Tests: 46/46 passing** 
- **Piece Tests: 82/82 passing**
- **Board Tests: 65/65 passing**

## Bug Fixes from Testing

The testing framework successfully identified and helped fix a critical bug:

**Bug Found**: The `Board::isValidMove()` function was missing validation to ensure that only the current player's pieces could be moved.

**Fix Applied**: Added validation in `Board::isValidMove()`:
```cpp
// Must be moving your own piece
if (fromPiece.getColor() != gameState.currentPlayer) return false;
```

This prevents players from moving their opponent's pieces, which is a fundamental chess rule.

## Test Categories

### Utils Tests
- ✅ String manipulation functions
- ✅ Chess coordinate conversions
- ✅ Move parsing and validation
- ✅ Algebraic notation handling

### Piece Tests  
- ✅ Piece construction and properties
- ✅ Correct piece values (centipawn system)
- ✅ Unicode symbol generation
- ✅ Movement rules for all piece types:
  - Pawn (including two-square initial move, diagonal capture)
  - Rook (horizontal/vertical movement)
  - Knight (L-shaped movement)
  - Bishop (diagonal movement)
  - Queen (combination of rook + bishop)
  - King (one square in any direction)

### Board Tests
- ✅ Proper starting position setup
- ✅ Board utilities (coordinate validation, square naming)
- ✅ Move validation and execution
- ✅ Turn management
- ✅ Check detection
- ✅ Legal move generation
- ✅ Game state management

## Adding New Tests

To add new tests:

1. **For existing test files**: Add new test functions and call them in the `main()` function
2. **For new test files**: 
   - Create `test_newfeature.cpp`
   - Follow the existing pattern with test functions
   - Add compilation rules to `Makefile`
   - Update main project `Makefile` if needed

### Example Test Function
```cpp
void test_new_feature() {
    // Arrange
    MyClass obj;
    
    // Act
    int result = obj.someMethod();
    
    // Assert
    TestFramework::assert_equal(42, result, "someMethod returns correct value");
    TestFramework::assert_true(obj.isValid(), "object is valid after method call");
}
```

## Benefits of the Testing Framework

1. **Quality Assurance** - Ensures code correctness and catches regressions
2. **Bug Detection** - Already found and helped fix a critical game logic bug
3. **Documentation** - Tests serve as executable documentation of expected behavior
4. **Refactoring Safety** - Allows confident code changes with immediate feedback
5. **Development Speed** - Quick verification of changes without manual testing

## Future Improvements

Potential enhancements for the testing framework:
- Integration tests for full game scenarios
- Performance benchmarks
- Mock objects for isolated testing
- Test coverage reporting
- Automated test execution in CI/CD

---

The testing framework provides comprehensive coverage of the chess game's core functionality and has already proven valuable in maintaining code quality and catching bugs early in development.
