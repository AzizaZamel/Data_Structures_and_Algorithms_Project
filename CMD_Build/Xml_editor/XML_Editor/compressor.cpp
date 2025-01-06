#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <tuple>
#include <sstream>
#include "xml_helper.hpp"
using namespace std;

const size_t BLOCK_SIZE = 32 * 1024; // 32KB

// Function to read file into blocks
vector<vector<char>> readFileIntoBlocks(const string& fileName) {
    ifstream inputFile(fileName, ios::binary);
    if (!inputFile.is_open()) {
        throw runtime_error("Error: Could not open the file!");
    }

    vector<vector<char>> blocks;
    char buffer[BLOCK_SIZE];
    size_t totalSize = 0;

    while (inputFile.read(buffer, BLOCK_SIZE) || inputFile.gcount() > 0) {
        size_t bytesRead = inputFile.gcount();
        totalSize += bytesRead;
        blocks.emplace_back(buffer, buffer + bytesRead);
    }

    inputFile.close();
    return blocks;
}

// LZ77 compression function
vector<tuple<int, int, char>> lz77Compress(const vector<char>& input) {
    int windowSize = 32 * 1024;  // Sliding window size
    int lookAheadBufferSize = 256; // Look-ahead buffer size
    int searchBufferSize = windowSize - lookAheadBufferSize;

    vector<tuple<int, int, char>> compressedData;  // Stores the tuples (offset, length, next_char)
    int i = 0;

    while (i < input.size()) {
        int maxLength = 0;
        int offset = 0;

        // Find the longest match in the search buffer
        for (int j = max(0, i - searchBufferSize); j < i; ++j) {
            int length = 0;
            while (length < lookAheadBufferSize && input[j + length] == input[i + length]) {
                ++length;
                if (i + length >= input.size()) break;
            }

            if (length > maxLength) {
                maxLength = length;
                offset = i - j;
            }
        }

        // If a match is found, store the tuple (offset, length, next_char)
        if (maxLength > 0) {
            compressedData.push_back(make_tuple(offset, maxLength, input[i + maxLength]));
            i += maxLength + 1;  // Skip over the matched part + next character
        } else {
            compressedData.push_back(make_tuple(0, 0, input[i]));
            ++i;  // No match found, store the literal character
        }
    }

    return compressedData;
}

// Function to serialize LZ77 compressed data into a string
string serializeLZ77(const vector<tuple<int, int, char>>& compressedData) {
    stringstream ss;
    for (const auto& tuple : compressedData) {
        ss << get<0>(tuple) << "," << get<1>(tuple) << "," << (int)get<2>(tuple) << " "; // Cast char to int for safe serialization
    }
    return ss.str();
}

// Function to write serialized LZ77 data to a file
void writeLZ77ToFile(const string& filename, const string& data) {
    ofstream outFile(filename, ios::binary);
    if (!outFile.is_open()) {
        cerr << "Unable to open output file!" << endl;
        return;
    }
    outFile.write(data.c_str(), data.size());
    outFile.close();
}

// Function to deserialize LZ77 compressed data from a string
vector<tuple<int, int, char>> deserializeLZ77(const string& data) {
    vector<tuple<int, int, char>> compressedData;
    stringstream ss(data);
    string token;

    while (getline(ss, token, ' ')) {
        if (token.empty()) continue;

        stringstream tupleStream(token);
        string offsetStr, lengthStr, nextCharStr;

        getline(tupleStream, offsetStr, ',');
        getline(tupleStream, lengthStr, ',');
        getline(tupleStream, nextCharStr, ',');

        int offset = stoi(offsetStr);
        int length = stoi(lengthStr);
        char nextChar = static_cast<char>(stoi(nextCharStr));

        compressedData.emplace_back(offset, length, nextChar);
    }

    return compressedData;
}

// LZ77 decompression function
string lz77Decompress(const vector<tuple<int, int, char>>& compressedData) {
    string decompressedData;

    for (const auto& tuple : compressedData) {
        int offset = get<0>(tuple);
        int length = get<1>(tuple);
        char nextChar = get<2>(tuple);

        // If offset and length are 0, this is a literal character
        if (offset == 0 && length == 0) {
            decompressedData += nextChar;
        } else {
            int start = decompressedData.size() - offset;
            for (int i = 0; i < length; ++i) {
                decompressedData += decompressedData[start + i];
            }
            decompressedData += nextChar;
        }
    }

    return decompressedData;
}

// Function to read a file's content into a string
string readFileToString(const string& filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile.is_open()) {
        throw runtime_error("Error: Could not open the file!");
    }

    stringstream buffer;
    buffer << inFile.rdbuf();
    inFile.close();
    return buffer.str();
}

void compressXML(const string& inputfilename , const string& outputfilename){
    vector<vector<char>> blocks = readFileIntoBlocks(inputfilename);
    cout << "Number of blocks: " << blocks.size() << endl;

    // Concatenate blocks into a single string for compression
    vector<char> allData;
    for (const auto& block : blocks) {
        allData.insert(allData.end(), block.begin(), block.end());
    }

    // Perform LZ77 compression
    auto compressedData = lz77Compress(allData);

    // Serialize compressed data into a string
    string serializedData = serializeLZ77(compressedData);

    // Write compressed data to a file

    writeLZ77ToFile(outputfilename, serializedData);
    cout << "Data compressed and saved to " << outputfilename << endl;
}

void decompressXMLfile(const string& inputfilename , const string& outputfilename){
    // Read compressed data from the file
    string compressedContent = readFileToString(inputfilename);

    // Deserialize compressed data
    auto deserializedData = deserializeLZ77(compressedContent);

    // Decompress the data
    string decompressedData = lz77Decompress(deserializedData);
    stringToFile(decompressedData,outputfilename);
    cout << "Decompression complete." << endl;



}
