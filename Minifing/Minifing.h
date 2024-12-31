/*
 ===================================================================================================
 Name        : Minify.h
 Author      : Doaa Said
 Description : header To declare functions that minify XML,the process involves removing
 unnecessary spaces,line breaks, and comments while retaining the essential structure.
 Date        : 28/11/2024
 ===================================================================================================
 */
#ifndef MINIFING_H_
#define MINIFING_H_

#include "xml_helper.h"
#include <iostream>
#include <stack>
#include <fstream>
#include <sstream>
#include <ostream>
#include <string>

using namespace std;

// Function to read the contents of a file and return it as a string.
//string convert_file_to_string(const string &filename);

// Function to write a string into an output file.
//void write_string_into_file(const string &output_filename, string output);

// Function to minify the XML content by removing unnecessary whitespace,
// comments, and declarations.
void minify_string(string str, string &output);

// Function to minify the content of the file by removing excess whitespace.
string minify_xml_file(const string &input_filename, const string &output_filename);



#endif /* MINIFING_H_ */
