#ifndef compressor
#define compressor

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

// Function to deserialize the Huffman Tree from a file
Node* deserializeTree(std::ifstream& inFile);

// Function to decompress the file and return the XML content
void decompressXML(const std::string& compressedFilename, const std::string& outputFilename);

// Function to build the Huffman Tree from frequency map
Node* buildHuffmanTree(const std::unordered_map<char, int>& freqMap);

// Function to generate Huffman codes from the tree
void generateHuffmanCodes(Node* root, const std::string& prefix, std::unordered_map<char, std::string>& huffmanCodes);

// Function to serialize the Huffman Tree into a file
void serializeTree(Node* root, std::ofstream& outFile);

// Function to compress an XML file using Huffman Coding
void compressXML(const std::string& inputFilename, const std::string& outputFilename);

#endif 
