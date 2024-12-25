#include <fstream>
#include <sstream>
#include <iostream>
#include "xml_checker.hpp"

// ANSI color codes for colored output
const std::string RESET_COLOR = "\033[0m";
const std::string GREEN_COLOR = "\033[32m";
const std::string RED_COLOR = "\033[31m";

// Helper function to read file content into a string
std::string readFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << RED_COLOR << "Failed to open file: " << filePath << RESET_COLOR << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Function to run a single test case
bool runTest(XmlChecker &checker, const std::string &testName, const std::string &filePath, int expectedErrors)
{
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Running " << testName << "..." << std::endl;

    std::string xmlContent = readFile(filePath);
    if (xmlContent.empty())
    {
        std::cout << RED_COLOR << testName << " Failed: Unable to read XML content." << RESET_COLOR << std::endl;
        return false;
    }

    int errors = checker.checkConsistency(xmlContent);
    if (errors == expectedErrors)
    {
        std::cout << GREEN_COLOR << testName << " Passed." << RESET_COLOR << std::endl;
        std::cout << GREEN_COLOR << "Expected Errors: " << expectedErrors << ", Detected Errors: " << errors << RESET_COLOR << std::endl;
        return true;
    }
    else
    {
        std::cout << RED_COLOR << testName << " Failed:" << RESET_COLOR << std::endl;
        std::cout << RED_COLOR << "Expected Errors: " << expectedErrors << ", Detected Errors: " << errors << RESET_COLOR << std::endl;
        return false;
    }
}

int main()
{
    XmlChecker checker;
    int passedTests = 0;
    int totalTests = 5;

    std::cout << "========== XML Consistency Checker Tests ==========" << std::endl;

    // Test Case 1: Valid XML
    if (runTest(checker, "Test Case 1: Valid XML", "examples/valid_example.xml", 0))
    {
        passedTests++;
    }

    // Test Case 2: Invalid XML - Missing Closing Tag
    if (runTest(checker, "Test Case 2: Invalid XML - Missing Closing Tag", "examples/invalid_missing_closing_tag.xml", 2))
    {
        passedTests++;
    }

    // Test Case 3: Invalid XML - Mismatched Tags
    if (runTest(checker, "Test Case 3: Invalid XML - Mismatched Tags", "examples/invalid_mismatched_tags.xml", 1))
    {
        passedTests++;
    }

    // Test Case 4: Invalid XML - Unexpected Closing Tag
    if (runTest(checker, "Test Case 4: Invalid XML - Unexpected Closing Tag", "examples/invalid_unexpected_closing_tag.xml", 2))
    {
        passedTests++;
    }

    // Test Case 5: Edge Case - Only Comments
    if (runTest(checker, "Test Case 5: Edge Case - Only Comments", "examples/edge_case_only_comments.xml", 0))
    {
        passedTests++;
    }

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Test Results: " << passedTests << " out of " << totalTests << " tests passed." << std::endl;

    if (passedTests == totalTests)
    {
        std::cout << GREEN_COLOR << "All tests passed successfully." << RESET_COLOR << std::endl;
    }
    else
    {
        std::cout << RED_COLOR << "Some tests failed." << RESET_COLOR << std::endl;
    }

    std::cout << "============================================" << std::endl;

    return 0;
}
