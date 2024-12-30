/*
 ============================================================================
 Name        : prettify.cpp
 Author      : Aziza Abdul Rahman Zamel
 Description : Source file to beautify/format XML files.
               Automatically indents and organizes the file for better readability and consistency.
 Date        : 1/12/2024
 ============================================================================
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "prettify.hpp"
#include "xml_helper.h"
#include <stdexcept>

#define INDENT_SPACES           4


/* This function takes an XML - like string as input and returns a prettified string as output.
 * It formats the input by adding indentation based on the nesting level of tags, handling both
 * self-closing and closing tags. The result is a string with properly indented tags and data.
*/
string prettifyString(const string& input_str) {
    ostringstream prettified;      // This will hold the formatted result.
    int depth = -1;                // Initialize depth of nesting (starting with -1 for the root element).
    int size = input_str.size();   // Get the length of the input string.
    bool comment_flag = false;     // Flag to detect if the current tag is a comment.
    bool first_line = true;        // Flag to manage formatting of the very first line.

    // Loop through the entire input string character by character.
    for (int i = 0; i < size; i++) {

        // Check if the current character is the start of a tag ('<').
        if (input_str[i] == '<') {

            // Handle newline before opening a new tag, except for the first line or short content.
            if (first_line) {
                // Disable the first line flag after processing.
                first_line = false;
            }
            else {
                prettified << '\n';
            }

            // If the current tag is not a closing tag or declaration, increase depth
            if ((input_str[i + 1] != '/') && (input_str[i + 1] != '?')) {
                depth++;

                // Set comment flag for comment tags (e.g., "<!--").
                if (input_str[i + 1] == '!') {
                    comment_flag = true;
                }
            }
            // Add the appropriate number of spaces for indentation based on depth.
            if (depth != -1) {
                prettified << string(depth * INDENT_SPACES, ' ');
            }

            // Loop until we find the end of the current tag (denoted by '>').
            while (input_str[i] != '>' && input_str[i] != '\0') {

                // If we find a self-closing tag ('/>') or a closing tag ('</'), decrease the depth.
                if (input_str[i] == '/' && ((input_str[i + 1] == '>') || (input_str[i - 1] == '<'))) {
                    depth--;
                }

                // Add the current character to the prettified output.
                prettified << input_str[i];

                // Handle indentation for multiline comment.
                if (input_str[i] == '\n' && depth != -1) {
                    prettified << string(depth * INDENT_SPACES, ' ');
                }

                // Move to the next character.
                i++;
            }
            // Append the closing '>' to the prettified output.
            prettified << '>';

            // decrease depth after processing a comment tag.
            if (comment_flag) {
                depth--;
                comment_flag = false;
            }

        }
        // Check if the character is not a space, tab, or newline (i.e., it's part of data).
        else if (input_str[i] != ' ' && input_str[i] != '\t' && input_str[i] != '\n') {

            // Add the appropriate number of spaces for indentation based on depth.
            prettified << '\n' << string((depth + 1) * INDENT_SPACES, ' ');

            // Collect all characters until encountering a '<', newline, or end of input.
            while (input_str[i] != '<' && input_str[i] != '\n' && input_str[i] != '\0') {

                // Add the current character to the prettified output.
                prettified << input_str[i];

                // Move to the next character.
                i++;
            }
            i--;
        }
    }

    // Return the formatted string.
    return prettified.str();

}

/* This function reads an XML - like file, prettifies its content, and writes the formatted result to another file.
 * It takes an input file name and an output file name as arguments, processes the content, and saves the prettified string.
*/
void prettifyXML(const string& inputFileName, const string& outputFileName) {
    // Read the content of the input file and store it in 'content'
    string content = fileToString(inputFileName);

    // Prettify the content by adding indentation and format it using the 'prettifyString' function
    string prettified = prettifyString(content);

    // Write the prettified content to the output file
    stringToFile(prettified, outputFileName);
}


// test prettify operation


//int main()
//{
//    try {
//        prettifyXML("example.txt", "output.txt");
//        string output = fileToString("output.txt");
//        cout<<output<<endl;
//
//    }
//    catch (const exception& e) {
//        cerr << "error: " << e.what() << std::endl;
//    }
//    return 0;
//}

