#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
using namespace std;

//************************************************************
//helper functions
bool is_start(string s) {
    if (s[0] == '<' && s[1] != '/') {
        return true;
    }
    else {
        return false;
    }
}

bool is_end(string s) {
    if (s[0] == '<' && s[1] == '/') {
        return true;
    }
    else {
        return false;
    }
}

bool is_string(string s) {
    for (int i = 1;i < s.size();i++) {
        if (s[0] == '<' && s[i] == '>') {
            return false;
        }
    }
    return true;
}

string start_string(string s) {
    return s.substr(1, s.size() - 2);
}

string end_string(string s) {
    return s.substr(2, s.size() - 3);
}

bool is_arr(vector<string>v, int a, int b, string g) {
    for (int i = a;i < v.size();i++) {
        if (is_start(v[i]) && is_end(v[i - 1])) {
            if ((end_string(v[i - 1]) == start_string(v[i])) && v[i] == g) {
                return true;
            }
        }
    }
    return false;
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

void convertXmlToJson(const string& inputFile, const string& outputFile) {
    vector<string> v;
    stack<string> bracket;
    //string parent;
    string tab = "\t";
    string b;

    //open xml file
    ifstream xmlFile(inputFile);
    if (!xmlFile.is_open()) {
        cerr << "Error opening file: " << inputFile << endl;
        return;
    }

    //create a json file
    ofstream jsonFile(outputFile);
    if (!jsonFile.is_open()) {
        cerr << "Error opening file: " << outputFile << endl;
        return;
    }

    //turn xml to a string and remove any excess space
    stringstream buffer;
    buffer << xmlFile.rdbuf();
    string xmlString = buffer.str();
    xmlFile.close();
    xmlString.erase(remove(xmlString.begin(), xmlString.end(), '\n'), xmlString.end());
    xmlString.erase(remove(xmlString.begin(), xmlString.end(), '\t'), xmlString.end());

    //cout << xmlString.length() << endl;
    //******************************************************************//
    //Get each tag frequency and divide words and put it in vector ;-;

    for (int i = 0; i < xmlString.size(); i++) {
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
        int start = i;
        while (i < xmlString.size() && xmlString[i] != '<') {
            i++;
        }
        v.push_back(xmlString.substr(start, i - start));
        i--;  // Adjust i to stay on the correct position (before '<' for next iteration)
        }
    }
    //******************************************************************//

    for (int i = 0; i < v.size();i++) {
        trimTrailingWhitespace(v[i]);
    }

    //start function from here
    //initial step
    //parent = start_string(v[0]);
    jsonFile << "{\n" + tab + "\"" + start_string(v[0]) + "\": ";
    int size = v.size();
    bracket.push("{");
    for (int i = 1; i < size - 1; i++) {
        if (is_start(v[i])) {
            bool prev_start_flag = is_start(v[i - 1]);      // 1 = start, 0 = end
            bool flag = is_arr(v, i, size, v[i - 1]);   // 1 = array, 0 = not an array
            bool already_exists = false;                      // did i already put this array tag or not
            if (i > 2 && is_start(v[i - 1]) && is_end(v[i - 2])) {
                if (start_string(v[i - 1]) == end_string(v[i - 2])) already_exists = true;
            }

            //start by checking if an array then checking if the previous is a start tag
            if (flag && !already_exists) {
                jsonFile << "[\n";
                bracket.push("[");
                tab += "\t";
            }

            if (prev_start_flag) {
                if (!bracket.empty() && bracket.top() == "[") {
                    jsonFile << tab + "{\n";
                }
                else {
                    jsonFile << "{\n";
                }
                tab += "\t";
                bracket.push("{");
                jsonFile << tab + "\"" + start_string(v[i]) + "\": ";
            }
            else if (end_string(v[i - 1]) != start_string(v[i])) {  //else must be end tag, if it's a string then the xml file is corrupt
                jsonFile << ",\n";
                jsonFile << tab + "\"" + start_string(v[i]) + "\": ";
            }
            else {
                jsonFile << ",\n";
            }

        }
        else if (is_end(v[i])) {

            if (!bracket.empty()) b = bracket.top();

            if ((start_string(v[i + 1]) == end_string(v[i])) && is_string(v[i-1]) && b == "[") {

            }
            else if (is_start(v[i + 1]) && (start_string(v[i + 1]) != end_string(v[i])) && is_string(v[i - 1])) {

            }
            else if ( is_string(v[i - 1]) && b != "[") {

            }
            else {
                if (!bracket.empty()) b = bracket.top();
                if (b == "[") {
                    if (!tab.empty()) tab.pop_back();
                    jsonFile << endl << tab + "]";

                }
                else if (b == "{") {
                    if (!tab.empty()) tab.pop_back();
                    jsonFile << endl << tab + "}";
                }
                if (!bracket.empty()) bracket.pop();
                if (!bracket.empty()) b = bracket.top();
                if (is_end(v[i + 1]) && b == "[") {
                    if (b == "[") {
                        if (!tab.empty()) tab.pop_back();
                        jsonFile << endl << tab + "]";

                    }
                    else if (b == "{") {
                        if (!tab.empty()) tab.pop_back();
                        jsonFile << endl << tab + "}";
                    }
                    if (!bracket.empty()) bracket.pop();
                }
            }
        }
        else {
            bool already_exists = false;
            if (start_string(v[i-1]) == end_string(v[i - 2])) already_exists = true;
            bool flag = is_arr(v, i, size, v[i - 1]);   // 1 = array, 0 = not an array
            if (flag && !already_exists) {
                jsonFile << "[\n";
                bracket.push("[");
                tab += "\t";
            }

            if (!bracket.empty() && bracket.top() == "[") {
                jsonFile << tab + "\"" + v[i] + "\"";
            }
            else {
                jsonFile << "\"" + v[i] + "\"";
            }
            
        }
    }

    while (!bracket.empty()) {
        b = bracket.top();
        if (b == "[") {
            if (!tab.empty()) tab.pop_back();
            jsonFile << "\n" + tab + "]";
            
        }
        else if (b == "{") {
            if (!tab.empty()) tab.pop_back();
            jsonFile << "\n" + tab + "}";
            
        }
        bracket.pop();
    }

    jsonFile << endl;
    cerr << "Conversion from XML to JSON completed!" << endl;
}


