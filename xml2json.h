#ifndef XML2JSON_H
#define XML2JSON_H

#include <stdbool.h> // For the boolean type if required in the function prototypes
#include <string>    // For std::string if using C++

// Function declarations
bool is_start(const char *str);
bool is_end(const char *str);
bool is_string(const char *str);
const char* start_string(const char *str);
const char* end_string(const char *str);
bool is_arr(const char *str);
void trimTrailingWhitespace(char *str);
char* convertXmlToJson(const char *xmlStr);

#endif // XML2JSON_H
