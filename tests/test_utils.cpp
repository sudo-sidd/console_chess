#include "test_framework.h"
#include "../include/Utils.h"
#include <iostream>

void test_string_utilities() {
    // Test trim function
    TestFramework::assert_equal("hello", ChessUtils::trim("  hello  "), "trim removes leading and trailing spaces");
    TestFramework::assert_equal("hello", ChessUtils::trim("hello"), "trim handles no spaces");
    TestFramework::assert_equal("", ChessUtils::trim("   "), "trim handles only spaces");
    TestFramework::assert_equal("hello world", ChessUtils::trim("\t\nhello world\r\n"), "trim removes various whitespace");
    
    // Test toLowerCase function
    TestFramework::assert_equal("hello world", ChessUtils::toLowerCase("HELLO WORLD"), "toLowerCase converts uppercase");
    TestFramework::assert_equal("hello world", ChessUtils::toLowerCase("Hello World"), "toLowerCase converts mixed case");
    TestFramework::assert_equal("hello123", ChessUtils::toLowerCase("HELLO123"), "toLowerCase preserves numbers");
    TestFramework::assert_equal("", ChessUtils::toLowerCase(""), "toLowerCase handles empty string");
    
    // Test split function
    auto result = ChessUtils::split("hello,world,test", ',');
    TestFramework::assert_equal(3, static_cast<int>(result.size()), "split returns correct number of parts");
    TestFramework::assert_equal("hello", result[0], "split first part correct");
    TestFramework::assert_equal("world", result[1], "split second part correct");
    TestFramework::assert_equal("test", result[2], "split third part correct");
    
    auto result2 = ChessUtils::split("  hello  ,  world  ", ',');
    TestFramework::assert_equal("hello", result2[0], "split trims whitespace");
    TestFramework::assert_equal("world", result2[1], "split trims whitespace");
    
    auto result3 = ChessUtils::split("single", ',');
    TestFramework::assert_equal(1, static_cast<int>(result3.size()), "split handles single item");
    TestFramework::assert_equal("single", result3[0], "split single item correct");
}

void test_coordinate_conversion() {
    // Test fileToCol
    TestFramework::assert_equal(0, ChessUtils::fileToCol('a'), "fileToCol 'a' = 0");
    TestFramework::assert_equal(1, ChessUtils::fileToCol('b'), "fileToCol 'b' = 1");
    TestFramework::assert_equal(7, ChessUtils::fileToCol('h'), "fileToCol 'h' = 7");
    
    // Test rankToRow
    TestFramework::assert_equal(0, ChessUtils::rankToRow('8'), "rankToRow '8' = 0");
    TestFramework::assert_equal(1, ChessUtils::rankToRow('7'), "rankToRow '7' = 1");
    TestFramework::assert_equal(7, ChessUtils::rankToRow('1'), "rankToRow '1' = 7");
    
    // Test colToFile
    TestFramework::assert_equal('a', ChessUtils::colToFile(0), "colToFile 0 = 'a'");
    TestFramework::assert_equal('b', ChessUtils::colToFile(1), "colToFile 1 = 'b'");
    TestFramework::assert_equal('h', ChessUtils::colToFile(7), "colToFile 7 = 'h'");
    
    // Test rowToRank
    TestFramework::assert_equal('8', ChessUtils::rowToRank(0), "rowToRank 0 = '8'");
    TestFramework::assert_equal('7', ChessUtils::rowToRank(1), "rowToRank 1 = '7'");
    TestFramework::assert_equal('1', ChessUtils::rowToRank(7), "rowToRank 7 = '1'");
}

void test_move_parsing() {
    int fromRow, fromCol, toRow, toCol;
    
    // Test valid moves
    bool result1 = ChessUtils::parseMove("e2 e4", fromRow, fromCol, toRow, toCol);
    TestFramework::assert_true(result1, "parseMove valid move 'e2 e4'");
    TestFramework::assert_equal(6, fromRow, "parseMove e2 fromRow");
    TestFramework::assert_equal(4, fromCol, "parseMove e2 fromCol");
    TestFramework::assert_equal(4, toRow, "parseMove e4 toRow");
    TestFramework::assert_equal(4, toCol, "parseMove e4 toCol");
    
    bool result2 = ChessUtils::parseMove("a1 h8", fromRow, fromCol, toRow, toCol);
    TestFramework::assert_true(result2, "parseMove valid move 'a1 h8'");
    TestFramework::assert_equal(7, fromRow, "parseMove a1 fromRow");
    TestFramework::assert_equal(0, fromCol, "parseMove a1 fromCol");
    TestFramework::assert_equal(0, toRow, "parseMove h8 toRow");
    TestFramework::assert_equal(7, toCol, "parseMove h8 toCol");
    
    // Test invalid moves
    TestFramework::assert_true(!ChessUtils::parseMove("e2", fromRow, fromCol, toRow, toCol), "parseMove rejects single square");
    TestFramework::assert_true(!ChessUtils::parseMove("e2 e4 e5", fromRow, fromCol, toRow, toCol), "parseMove rejects three squares");
    TestFramework::assert_true(!ChessUtils::parseMove("z1 a2", fromRow, fromCol, toRow, toCol), "parseMove rejects invalid file");
    TestFramework::assert_true(!ChessUtils::parseMove("a9 a2", fromRow, fromCol, toRow, toCol), "parseMove rejects invalid rank");
    TestFramework::assert_true(!ChessUtils::parseMove("", fromRow, fromCol, toRow, toCol), "parseMove rejects empty string");
}

void test_move_to_string() {
    std::string result1 = ChessUtils::moveToString(6, 4, 4, 4);
    TestFramework::assert_equal("e2 e4", result1, "moveToString e2 e4");
    
    std::string result2 = ChessUtils::moveToString(7, 0, 0, 7);
    TestFramework::assert_equal("a1 h8", result2, "moveToString a1 h8");
    
    std::string result3 = ChessUtils::moveToString(0, 0, 7, 7);
    TestFramework::assert_equal("a8 h1", result3, "moveToString a8 h1");
}

// Main function for standalone execution
int main() {
    std::cout << "Running Utils Tests" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    TestFramework::run_test("String Utilities", test_string_utilities);
    TestFramework::run_test("Coordinate Conversion", test_coordinate_conversion);
    TestFramework::run_test("Move Parsing", test_move_parsing);
    TestFramework::run_test("Move to String", test_move_to_string);
    
    TestFramework::print_summary();
    
    return TestFramework::all_tests_passed() ? 0 : 1;
}
