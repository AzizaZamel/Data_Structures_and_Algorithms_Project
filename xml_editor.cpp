#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <queue>
#include <stack>
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

string start_string(string s) {
    return s.substr(1, s.size() - 2);
}

string end_string(string s) {
    return s.substr(2, s.size() - 3);
}

int child_count(vector<string>v, int a, int b ,string g) {
    string s = start_string(g);
    int count = 0;
    for (int i = a;i < b;i++) {
        if (start_string(v[i]) == s) {
            count++;
        }
    }
    return count;
}

bool is_arr(vector<string>v, int a, int b, string g) {
    for (int i = 1;i < v.size();i++) {
        if (is_start(v[i]) && is_end(v[i - 1])) {
            if (end_string(v[i - 1]) == start_string(v[i]) && v[i] == g) {
                return true;
            }
        }
    }
    return false;
}
//************************************************************

/*string child(string parent, string tab, vector<string> vec, int a, int b) {
    map<string, int> m;
    bool flag_leaf = true;
    string s;

    if (is_start(vec[a])) {
        for (int i = a;i < b;i++) {
            if (start_string(vec[i])== start_string(vec[a])) {
                m[vec[a]]++;        //counter up in map
            }
        }
    }
    else {
        return vec[a];
    }

    auto it = m.find(vec[a]);
    int value = it->second;     //get child freq

    if (value == 1) {       //not array
        return "\"" + vec[a] + "\": {\n" + tab + child(vec[a], tab, vec, a+1, b);
    }
    else {      //array - assuming all array elements are after each other
        for (int i = a;i < b;i++) {
            if (start_string(vec[i]) != start_string(vec[a])) {
                cerr << "here";
                s = s + child(vec[0], tab, vec, a + 1, b - 1);
            }
            else {
                return s;
            }
        }
    }
}*/

void convertXmlToJson(const string& inputFile, const string& outputFile) {
    vector<string> v;
    stack<string> s;
    string parent;
    string tab = "\t\t\t";

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
        cerr << v[i] << ",";
    }

    //start function from here
    //initial step
    parent = start_string(v[0]);
    jsonFile << "{\n\t" << "\"" << parent << "\": {\n\t\t";
    int size = v.size();
    for (int i = 1; i < size; i++) {
        if (is_start(v[i])) {

            s.push(v[i]);
            jsonFile << "\"" + start_string(v[i]) + "\"";
            bool flag = is_arr(v, i, size, v[i]);
            //bool flag = false;

            if (!flag) {
                jsonFile << ": {\n\t\t\n";
            }
            else {
                jsonFile << ": [\n\t\t{\t\t\t\n";
            }

        }
        else if (is_end(v[i])) {

            string end = s.top();
            int eCount = child_count(v, i, size, v[i]);
            if (eCount == 1) {
                jsonFile << "}\n\t\t";
            }
            else {
                jsonFile << "]\n\t\t}\t\t\t";
            }

        }
        else {
            jsonFile << "\"" + v[i] + "\"";
            //cerr << v[i] << endl;
        }
    }

    //jsonFile << child(parent, tab, v, 1, v.size()-2);
    jsonFile << endl;
    cerr << "Conversion from XML to JSON completed!" << endl;
}


int main(int argc, char* argv[]) {
    /*if (argc < 5) {
        cerr << "usage: xml_editor json -i sample.xml -o output.json" << endl;
        return 1;
    }

    string mode = argv[1];
    if (mode != "json") {
        cerr << "Invalid mode. Only 'json' is supported." << endl;
        return 1;
    }*/
    cerr << "json mode, yay!" << endl;
    // Parse the input file and output file from command-line arguments
    //string inputFile = argv[3];
    //string outputFile = argv[5];

    string inputFile = "C:/Users/Acer/Desktop/Hana/xml_editor/x64/Debug/sample.xml";
    string outputFile = "output.json";

    // Call the function to convert XML to JSON
    convertXmlToJson(inputFile, outputFile);

    return 0;
}