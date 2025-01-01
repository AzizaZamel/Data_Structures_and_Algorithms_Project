#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <string>


// Define the Huffman Node structure
#include"decompressor.h"

// Deserialize the Huffman Tree from a file
Node2* deserializeTree(std::ifstream& inFile) {
    char type;
    inFile.get(type);

    if (type == '1') {
        char character;
        inFile.get(character);
        return new Node2(character, 0);  // Leaf node
    } else {
        Node2* left = deserializeTree(inFile);
        Node2* right = deserializeTree(inFile);
        return new Node2(left, right);  // Internal node
    }
}

// Decompress the file and return the XML content
void decompressXML(const std::string& compressedFilename, const std::string& outputFilename) {
    std::ifstream inFile(compressedFilename, std::ios::binary);

    // Deserialize the Huffman tree
    Node2* root = deserializeTree(inFile);

    // Read the padding size
    char padding;
    inFile.get(padding);

    // Read the bitstream
    std::string bitstream;
    char byte;
    while (inFile.get(byte)) {
        std::bitset<8> bits(static_cast<unsigned long long>(byte));
        bitstream += bits.to_string();
    }

    // Remove padding
    bitstream = bitstream.substr(0, bitstream.size() - padding);

    // Decode the bitstream using the Huffman tree
    std::string decodedText;
    Node2* current = root;
    for (char bit : bitstream) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        // If it's a leaf node, append the character and reset the tree
        if (current->left == nullptr && current->right == nullptr) {
            decodedText += current->character;
            current = root;
        }
    }

    // Write the decompressed XML content to a new file
    std::ofstream outFile(outputFilename);
    outFile << decodedText;
    outFile.close();

    std::cout << "Decompression completed!" << std::endl;
}

// Example Usage


/*int main() {
    std::string compressedFile = "compressed_file_test.huff";
    std::string outputXML = "output.xml";

    // Decompress the file and output to a new XML file
    decompressXML(compressedFile, outputXML);

    return 0;
}*/
