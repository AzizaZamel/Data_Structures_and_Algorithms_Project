#ifndef decompressor
#define decompressor

#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <string>

// Define the Huffman Node structure
struct Node2 {
    char character;
    int frequency;
    Node2* left;
    Node2* right;

    Node2(char c, int freq) : character(c), frequency(freq), left(nullptr), right(nullptr) {}
    Node2(Node2* left, Node2* right) : character(0), frequency(left->frequency + right->frequency), left(left), right(right) {}

    bool operator>(const Node2& other) const {
        return frequency > other.frequency;
    }
};

// Deserialize the Huffman Tree from a file
Node2* deserializeTree2(std::ifstream& inFile);

// Decompress the file and return the XML content
void decompressXML(const std::string& compressedFilename, const std::string& outputFilename);

#endif
