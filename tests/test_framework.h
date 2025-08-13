#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <sstream>

class TestFramework {
private:
    static int tests_run;
    static int tests_passed;
    static std::vector<std::string> failed_tests;

public:
    static void assert_true(bool condition, const std::string& test_name) {
        tests_run++;
        if (condition) {
            tests_passed++;
            std::cout << "✓ " << test_name << std::endl;
        } else {
            failed_tests.push_back(test_name);
            std::cout << "✗ " << test_name << std::endl;
        }
    }

    static void assert_equal(int expected, int actual, const std::string& test_name) {
        tests_run++;
        if (expected == actual) {
            tests_passed++;
            std::cout << "✓ " << test_name << std::endl;
        } else {
            failed_tests.push_back(test_name + " (expected: " + std::to_string(expected) + ", got: " + std::to_string(actual) + ")");
            std::cout << "✗ " << test_name << " (expected: " << expected << ", got: " << actual << ")" << std::endl;
        }
    }

    static void assert_equal(const std::string& expected, const std::string& actual, const std::string& test_name) {
        tests_run++;
        if (expected == actual) {
            tests_passed++;
            std::cout << "✓ " << test_name << std::endl;
        } else {
            failed_tests.push_back(test_name + " (expected: '" + expected + "', got: '" + actual + "')");
            std::cout << "✗ " << test_name << " (expected: '" << expected << "', got: '" << actual << "')" << std::endl;
        }
    }

    static void run_test(const std::string& test_name, std::function<void()> test_function) {
        std::cout << "\n--- Running " << test_name << " ---" << std::endl;
        try {
            test_function();
        } catch (const std::exception& e) {
            tests_run++;
            failed_tests.push_back(test_name + " (exception: " + e.what() + ")");
            std::cout << "✗ " << test_name << " (exception: " << e.what() << ")" << std::endl;
        }
    }

    static void print_summary() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "TEST SUMMARY" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "Tests run: " << tests_run << std::endl;
        std::cout << "Tests passed: " << tests_passed << std::endl;
        std::cout << "Tests failed: " << (tests_run - tests_passed) << std::endl;
        
        if (!failed_tests.empty()) {
            std::cout << "\nFailed tests:" << std::endl;
            for (const auto& test : failed_tests) {
                std::cout << "  - " << test << std::endl;
            }
        }
        
        if (tests_passed == tests_run) {
            std::cout << "\n🎉 All tests passed!" << std::endl;
        } else {
            std::cout << "\n❌ Some tests failed." << std::endl;
        }
    }

    static void reset() {
        tests_run = 0;
        tests_passed = 0;
        failed_tests.clear();
    }

    // Public accessors for test counts
    static int get_tests_run() { return tests_run; }
    static int get_tests_passed() { return tests_passed; }
    static bool all_tests_passed() { return tests_run == tests_passed; }
};

// Static member definitions
int TestFramework::tests_run = 0;
int TestFramework::tests_passed = 0;
std::vector<std::string> TestFramework::failed_tests;
