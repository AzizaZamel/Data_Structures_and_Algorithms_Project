#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <list>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include "xml_helper.hpp"
using namespace std;

bool is_start(string s) {   //checks if string is a start tag
    if (s[0] == '<' && s[1] != '/') {
        return true;
    }
    else {
        return false;
    }
}

bool is_end(string s) {     //checks if string is an end tag
    if (s[0] == '<' && s[1] == '/') {
        return true;
    }
    else {
        return false;
    }
}

bool is_string(string s) {      //checks if string is data
    for (unsigned int i = 1;i < s.size();i++) {
        if (s[0] == '<' && s[i] == '>') {
            return false;
        }
    }
    return true;
}

string start_string(string s) {     //removes start tag's "<>"
    return s.substr(1, s.size() - 2);
}

string end_string(string s) {       //removes end tag's "</>"
    return s.substr(2, s.size() - 3);
}

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
void stringToFile(string& content, const string& filename) {
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

void trimTrailingWhitespace(string& str) {
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    if (end != string::npos) {
        str.erase(end + 1);
    }
    else {
        str.clear();
    }
}

//************************************************************

vector<string> parser(const string& inputFile) {
    vector<string> v;

    string xmlString = fileToString(inputFile);
    xmlString.erase(remove(xmlString.begin(), xmlString.end(), '\n'), xmlString.cend());
    xmlString.erase(remove(xmlString.begin(), xmlString.end(), '\t'), xmlString.cend());

    // Get each tag and store it in the vector
    for (unsigned int i = 0; i < xmlString.size(); i++) {
        int word_size = 0;
        if (xmlString[i] == '<') {
            word_size++;
            while (xmlString[i] != '>') {
                i++;
                word_size++;
            }
            v.push_back(xmlString.substr(i - word_size + 1, word_size)); // Exclude '<' and '>'
        }
        else if (xmlString[i] == ' ') continue;
        else {
            unsigned int start = i;
            while ( i < xmlString.size() && xmlString[i] != '<') {
                i++;
            }
            v.push_back(xmlString.substr(start, i - start));
            i--;  // Adjust i to stay on the correct position (before '<' for next iteration)
        }
    }

    // Trim trailing spaces
    for (unsigned int i = 0; i < v.size(); i++) {
        trimTrailingWhitespace(v[i]);
    }
    return v;
}
