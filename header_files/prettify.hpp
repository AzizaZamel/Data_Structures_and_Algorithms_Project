/*
 ============================================================================
 Name        : prettify.hpp
 Author      : Aziza Abdul Rahman Zamel
 Description : Header file to beautify/format XML files.
               Automatically indents and organizes the file for better readability and consistency.
 Date        : 1/12/2024
 ============================================================================
 */

#ifndef _PRETTIFY_
#define _PRETTIFY_

#include <string>
using namespace std;


/* Function to read the contents of a file and return it as a string */
string fileToString(const string& filename);

/* Function to write the contents of a string to a file */
void stringToFile(string& content, const string& filename);

/* This function takes an XML - like string as input and returns a prettified string as output.
 * It formats the input by adding indentation based on the nesting level of tags, handling both
 * self-closing and closing tags. The result is a string with properly indented tags and data.
*/
string prettifyString(const string& input_str);

/* This function reads an XML - like file, prettifies its content, and writes the formatted result to another file.
 * It takes an input file name and an output file name as arguments, processes the content, and saves the prettified string.
*/
void prettifyXML(const string& inputFileName, const string& outputFileName);


#endif