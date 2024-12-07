#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <queue>
using namespace std;

string child(string in, string parent, string tab, map<string, int> myMap, vector<string> vec) {
    string cur = vec[1];
    vector<string> v(vec.begin() + 1, vec.end());
    int count;
    if (cur[0] == '<') {    //If word is a tag
        auto it = myMap.find(cur.substr(1, cur.size() - 2));
        int value = it->second;     //get current word freq
        if (value == 1) {
            in = in + "\"" + cur.substr(1, cur.size() - 2) + "\": {\n" + tab;
            in = in + child(in, cur, tab, myMap, v);
        }
        else {
            in = in + "\"" + cur.substr(1, cur.size() - 2) + "\": [\n" + tab + "{\n";
            tab = tab + "\t";
            in = in + tab;
            for (int i = 0; i < value; i++) {
                auto index = find(v.begin(), v.end(), cur);
                string huh = *index;
                while (huh.substr(1, cur.size() - 2) != "\"" + cur) {

                }
            }
        }
    }
}

void convertXmlToJson(const string& inputFile, const string& outputFile) {
    vector<string> v;
    map<string, int> wordCount;     //get each tag frequency
    string parent;
    string tab = "\t\t\t";
    queue<string> out;

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
    xmlString.erase(remove(xmlString.begin(), xmlString.end(), '\n'), xmlString.cend());

    //******************************************************************//
    //Get each tag frequency and divide words and put it in vector ;-;

    for (int i = 0; i < xmlString.size(); i++) {
        int word_size = 0;    //reset word size
        cerr << "i = " << i << endl;
        if (xmlString[i++] == '<') {
            
            //skip first part
            if (xmlString[i++] == '?') {
                while (xmlString[i++] != '>');
                i--;  // to remove excess i from xmlString[i++]
                continue;
            }

            while (xmlString[i++] != '>') word_size++;
            i--;
            cerr << xmlString.substr(i - word_size - 1, word_size + 1);
            if (xmlString[i - word_size - 1] != '/') {
                wordCount[xmlString.substr(i - word_size - 1, word_size + 1)]++;    //add tag to map or increase count to exsiting word
                v.push_back(xmlString.substr(i - word_size - 2, word_size + 3));    //adds tag with "<>" to vector
            }
            else {
                v.push_back(xmlString.substr(i - word_size - 2, word_size + 3));    //adds end tag to vector
            }
        }
        else if (xmlString[i] == ' ') continue;
        else {
            cerr << "heo ";
            i++;
            while (xmlString[i++] != '<' && i < xmlString.size()) word_size++;
            i = i - 2;
            v.push_back(xmlString.substr(i - word_size - 1, word_size + 2));
        }
    }
    //******************************************************************//

    //start function from here
    //initial step
    parent = v[0].substr(1, v[0].size() - 2);
    jsonFile << "{\n\t" << "\"" << parent << "\": {\n\t\t";
    string output;
    jsonFile << child(output, parent, tab, wordCount, v);

    for (const auto& pair : wordCount) {
        jsonFile << pair.first << ": " << pair.second << endl;
    }
    for (int i = 0; i < v.size(); i++) {
        jsonFile << v[i] << ",";
    }
    jsonFile << endl;
    jsonFile << xmlString;
    cout << "Conversion from XML to JSON completed!" << endl;
}


int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "usage: xml_editor json -i input_file.xml -o output_file.json" << endl;
        return 1;
    }

    string mode = argv[1];
    if (mode != "json") {
        cerr << "Invalid mode. Only 'json' is supported." << endl;
        return 1;
    }
    cerr << "json mode, yay!";
    // Parse the input file and output file from command-line arguments
    string inputFile = argv[3];
    string outputFile = argv[5];

    // Call the function to convert XML to JSON
    convertXmlToJson(inputFile, outputFile);

    return 0;
}