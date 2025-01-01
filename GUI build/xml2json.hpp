#ifndef XML2JSON_H
#define XML2JSON_H

#include <stdbool.h> // For the boolean type if required in the function prototypes
#include <string> // For std::string if using C++
#include <vector>
using namespace std;
// Function declarations
bool is_arr(vector<string>v, int a, int b, string g);
void convertXmlToJson(const string& inputFile, const string& outputFile);

#endif // XML2JSON_H
