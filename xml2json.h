#ifndef XML2JSON_H
#define XML2JSON_H

#include <stdbool.h> // For the boolean type if required in the function prototypes
#include <string> // For std::string if using C++
#include <vector>
using namespace std;
// Function declarations
bool is_start(string s);
bool is_end(string s);
bool is_string(string s) ;
string start_string(string s);
string end_string(string s);
bool is_arr(vector<string>v, int a, int b, string g);
void trimTrailingWhitespace(string& str);
void convertXmlToJson(const string& inputFile, const string& outputFile);

#endif // XML2JSON_H
