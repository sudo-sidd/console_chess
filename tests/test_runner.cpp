#include "test_framework.h"
#include <iostream>
#include <cstdlib>

// Forward declarations for test main functions
int test_utils_main();
int test_piece_main();
int test_board_main();

// Individual test runners that return exit codes
int test_utils_main() {
    TestFramework::reset();
    std::cout << "Running Utils Tests" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    // Include test functions from test_utils.cpp
    extern void test_string_utilities();
    extern void test_coordinate_conversion();
    extern void test_move_parsing();
    extern void test_move_to_string();
    
    TestFramework::run_test("String Utilities", test_string_utilities);
    TestFramework::run_test("Coordinate Conversion", test_coordinate_conversion);
    TestFramework::run_test("Move Parsing", test_move_parsing);
    TestFramework::run_test("Move to String", test_move_to_string);
    
    TestFramework::print_summary();
    return (TestFramework::tests_run == TestFramework::tests_passed) ? 0 : 1;
}

int test_piece_main() {
    TestFramework::reset();
    std::cout << "Running Piece Tests" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    // Include test functions from test_piece.cpp
    extern void test_piece_construction();
    extern void test_piece_values();
    extern void test_piece_symbols();
    extern void test_pawn_movement();
    extern void test_rook_movement();
    extern void test_knight_movement();
    extern void test_bishop_movement();
    extern void test_queen_movement();
    extern void test_king_movement();
    extern void test_move_structure();
    
    TestFramework::run_test("Piece Construction", test_piece_construction);
    TestFramework::run_test("Piece Values", test_piece_values);
    TestFramework::run_test("Piece Symbols", test_piece_symbols);
    TestFramework::run_test("Pawn Movement", test_pawn_movement);
    TestFramework::run_test("Rook Movement", test_rook_movement);
    TestFramework::run_test("Knight Movement", test_knight_movement);
    TestFramework::run_test("Bishop Movement", test_bishop_movement);
    TestFramework::run_test("Queen Movement", test_queen_movement);
    TestFramework::run_test("King Movement", test_king_movement);
    TestFramework::run_test("Move Structure", test_move_structure);
    
    TestFramework::print_summary();
    return (TestFramework::tests_run == TestFramework::tests_passed) ? 0 : 1;
}

int test_board_main() {
    TestFramework::reset();
    std::cout << "Running Board Tests" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    // Include test functions from test_board.cpp
    extern void test_board_initialization();
    extern void test_board_utilities();
    extern void test_basic_move_validation();
    extern void test_move_execution();
    extern void test_piece_setting();
    extern void test_check_detection();
    extern void test_legal_moves_generation();
    extern void test_board_reset();
    extern void test_game_state_structure();
    
    TestFramework::run_test("Board Initialization", test_board_initialization);
    TestFramework::run_test("Board Utilities", test_board_utilities);
    TestFramework::run_test("Basic Move Validation", test_basic_move_validation);
    TestFramework::run_test("Move Execution", test_move_execution);
    TestFramework::run_test("Piece Setting", test_piece_setting);
    TestFramework::run_test("Check Detection", test_check_detection);
    TestFramework::run_test("Legal Moves Generation", test_legal_moves_generation);
    TestFramework::run_test("Board Reset", test_board_reset);
    TestFramework::run_test("GameState Structure", test_game_state_structure);
    
    TestFramework::print_summary();
    return (TestFramework::tests_run == TestFramework::tests_passed) ? 0 : 1;
}

int main(int argc, char* argv[]) {
    std::cout << "Chess Game Test Suite" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    int total_failures = 0;
    
    // Run individual test suites
    std::cout << "\n" << std::string(70, '-') << std::endl;
    total_failures += test_utils_main();
    
    std::cout << "\n" << std::string(70, '-') << std::endl;
    total_failures += test_piece_main();
    
    std::cout << "\n" << std::string(70, '-') << std::endl;
    total_failures += test_board_main();
    
    // Final summary
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "FINAL TEST RESULTS" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    if (total_failures == 0) {
        std::cout << "🎉 ALL TESTS PASSED! 🎉" << std::endl;
        std::cout << "The chess game components are working correctly." << std::endl;
    } else {
        std::cout << "❌ " << total_failures << " test suite(s) had failures." << std::endl;
        std::cout << "Please review the individual test results above." << std::endl;
    }
    
    return total_failures;
}
