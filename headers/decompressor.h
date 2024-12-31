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
struct Node {
    char character;
    int frequency;
    Node* left;
    Node* right;

    Node(char c, int freq);
    Node(Node* left, Node* right);

    bool operator>(const Node& other) const;
};

// Deserialize the Huffman Tree from a file
Node* deserializeTree(std::ifstream& inFile);

// Decompress the file and return the XML content
void decompressXML(const std::string& compressedFilename, const std::string& outputFilename);

#endif