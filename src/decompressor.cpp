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

    Node(char c, int freq) : character(c), frequency(freq), left(nullptr), right(nullptr) {}
    Node(Node* left, Node* right) : character(0), frequency(left->frequency + right->frequency), left(left), right(right) {}

    bool operator>(const Node& other) const {
        return frequency > other.frequency;
    }
};

// Deserialize the Huffman Tree from a file
Node* deserializeTree(std::ifstream& inFile) {
    char type;
    inFile.get(type);

    if (type == '1') {
        char character;
        inFile.get(character);
        return new Node(character, 0);  // Leaf node
    } else {
        Node* left = deserializeTree(inFile);
        Node* right = deserializeTree(inFile);
        return new Node(left, right);  // Internal node
    }
}

// Decompress the file and return the XML content
void decompressXML(const std::string& compressedFilename, const std::string& outputFilename) {
    std::ifstream inFile(compressedFilename, std::ios::binary);

    // Deserialize the Huffman tree
    Node* root = deserializeTree(inFile);

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
    Node* current = root;
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
int main() {
    std::string compressedFile = "compressed_file_test.huff";
    std::string outputXML = "output.xml";

    // Decompress the file and output to a new XML file
    decompressXML(compressedFile, outputXML);

    return 0;
}
