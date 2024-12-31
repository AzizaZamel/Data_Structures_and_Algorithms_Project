#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <map>
#include <string>

using namespace std;

// Node structure for Huffman Tree
struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f);
};

// Comparator for priority queue
struct Compare {
    bool operator()(Node* left, Node* right);
};

// Function to generate Huffman codes from the tree
void generateCodes(Node* root, const string& code,const map<char, string>& huffmanCodes);

// Function to build Huffman tree from character frequencies
Node* buildHuffmanTree(const map<char, int>& frequencies);

// Function to write compressed data to a binary file
void writeCompressedFile(const string& input,const map<char, string>& huffmanCodes,const string& outputFile);

// Function to convert file contents to string stream
string filetoss(const string& filename);

// Main compression function
void compressXML(const string& inputFile, const string& outputFile);

#endif // COMPRESSOR_H
