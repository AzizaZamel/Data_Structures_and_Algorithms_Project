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
#include <stdexcept>

#define INDENT_SPACES   4

/* Function to read the contents of a file and return it as a string */
string fileToString(const string& filename) {
    // Open the file
    ifstream file(filename); 
    // Check if the file is successfully opened
    if (!file.is_open()) {
        // If the file couldn't be opened, throw an exception with an error message
        throw std::runtime_error("Can't open file: " + filename);
    }

    // Create a stringstream to hold the file contents
    stringstream buffer;
    // Read file contents into buffer
    buffer << file.rdbuf(); 

    file.close();        // Close the file
    return buffer.str(); // Return the file contents as a string
}

/* Function to write the contents of a string to a file */
void stringToFile(string& content,const string& filename) {
    // Create an ofstream object to open the file with the given filename
    ofstream fileOut(filename);
    // Check if the file is successfully opened
    if (!fileOut.is_open()) {
        // If the file couldn't be opened, print an error message and return 
        std::cerr << "Error: Could not open output file: " << filename << std::endl;
        return;
    }

    // Write the content of the string to the file
    fileOut << content;

    // Close the file
    fileOut.close();
}

/* This function takes an XML - like string as input and returns a prettified string as output.
 * It formats the input by adding indentation based on the nesting level of tags, handling both 
 * self-closing and closing tags. The result is a string with properly indented tags and data.
*/
string prettifyString(const string& input_str) {
    ostringstream prettified;      // This will hold the formatted result.
    int depth = -1;                // Initialize depth of nesting (starting with -1 for the root element).
    int size = input_str.size();   // Get the length of the input string.

    // Loop through the entire input string character by character.
    for (int i = 0; i < size; i++) {

        // Check if the current character is the start of a tag ('<').
        if (input_str[i] == '<') {

            // If the current tag is not a closing tag (i.e., not '</'), increase the depth.
            if (input_str[i + 1] != '/') {
                depth++;
            }
            // Add the appropriate number of spaces for indentation based on depth.
            prettified << string(depth * INDENT_SPACES, ' ');

            // Loop until we find the end of the current tag (denoted by '>').
            while (input_str[i] != '\n' && input_str[i]!='\0') {

                // If we find a self-closing tag ('/>') or a closing tag ('</'), decrease the depth.
                if (input_str[i] == '/' && ((input_str[i + 1] == '>') || (input_str[i - 1] == '<'))) {
                    depth--;
                }

                // Add the current character to the prettified output.
                prettified << input_str[i];

                // Break the loop once the closing '>' of the tag is found.
                if (input_str[i] == '>') {
                    break;
                }

                // Move to the next character.
                i++;
            }

            // Add a newline after the tag is processed to separate it from subsequent content.
            prettified << '\n';
            
        }
        // Check if the character is not a space, tab, or newline (i.e., it's part of data).
        else if (input_str[i] != ' ' && input_str[i] != '\t' && input_str[i] != '\n') { 

            // Add indentation based on depth for data inside elements.
            prettified << string((depth+1) * INDENT_SPACES, ' ');

            // Loop to process all characters that are part of the data until the next tag.
            while (input_str[i] != '\n' && input_str[i] != '\0') {
                prettified << input_str[i];

                // If the next character indicates the start of a new tag, break out of the loop.
                if (input_str[i + 1] == '<') {
                    break;
                }

                // Move to the next character.
                i++;
            }

            // Add a newline after the data to keep it properly formatted.
            prettified << '\n';
        }
    }

    // Return the formatted string.
    return prettified.str();

}

/* This function reads an XML - like file, prettifies its content, and writes the formatted result to another file.
 * It takes an input file name and an output file name as arguments, processes the content, and saves the prettified string.
*/
void prettifyXML(const string& inputFileName,const string& outputFileName) {
    // Read the content of the input file and store it in 'content'
    string content = fileToString(inputFileName);

    // Prettify the content by adding indentation and format it using the 'prettifyString' function
    string prettified = prettifyString(content);

    // Write the prettified content to the output file
    stringToFile(prettified, outputFileName);
}


// test prettify operation

/*
int main()
{
    try {
        prettifyXML("example.txt", "output.txt");
        string output = fileToString("output.txt");
        cout<<output<<endl;

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
*/
