#ifndef XML_HELPER_H    // Include guard using the file name
#define XML_HELPER_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool is_start(string s);	//checks if string is a start tag

bool is_end(string s);		//checks if string is an end tag

bool is_string(string s);   //checks if string is data

string start_string(string s);	//removes start tag's "<>"

string end_string(string s);	//removes end tag's "</>"

string fileToString(const string& filename);

void stringToFile(string& content, const string& filename);

void trimTrailingWhitespace(string& str);	//removes whitespace at the end of a string

vector<string> parser(const string& inputFile);

#endif