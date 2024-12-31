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

// Build the Huffman Tree
Node* buildHuffmanTree(const std::unordered_map<char, int>& freqMap) {
    std::priority_queue<Node*, std::vector<Node*>, std::greater<Node*>> minHeap;

    // Push all characters with their frequencies into the priority queue
    for (const auto& entry : freqMap) {
        minHeap.push(new Node(entry.first, entry.second));
    }

    // Build the tree by combining the two nodes with the least frequencies
    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();
        minHeap.push(new Node(left, right));
    }

    return minHeap.top();  // Root of the Huffman tree
}

// Generate the Huffman codes by traversing the tree
void generateHuffmanCodes(Node* root, const std::string& prefix, std::unordered_map<char, std::string>& huffmanCodes) {
    if (root == nullptr) return;

    // If leaf node, store the code for the character
    if (root->left == nullptr && root->right == nullptr) {
        huffmanCodes[root->character] = prefix;
    }

    // Traverse left and right subtrees
    generateHuffmanCodes(root->left, prefix + "0", huffmanCodes);
    generateHuffmanCodes(root->right, prefix + "1", huffmanCodes);
}

// Serialize the Huffman Tree into a string
void serializeTree(Node* root, std::ofstream& outFile) {
    if (root == nullptr) return;
    
    if (root->left == nullptr && root->right == nullptr) {
        outFile.put('1');  // Leaf node indicator
        outFile.put(root->character);
    } else {
        outFile.put('0');  // Internal node indicator
        serializeTree(root->left, outFile);
        serializeTree(root->right, outFile);
    }
}

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

// Compress the XML file using Huffman Coding
void compressXML(const std::string& inputFilename, const std::string& outputFilename) {
    // Read the XML file into a string
    std::ifstream inputFile(inputFilename, std::ios::binary);
    std::string inputText((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    // Calculate frequencies of characters
    std::unordered_map<char, int> freqMap;
    for (char ch : inputText) {
        freqMap[ch]++;
    }

    // Build Huffman tree
    Node* root = buildHuffmanTree(freqMap);

    // Generate Huffman codes for each character
    std::unordered_map<char, std::string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    // Create the output bitstream
    std::string bitstream;
    for (char ch : inputText) {
        bitstream += huffmanCodes[ch];
    }

    // Add padding if necessary to make the bitstream a multiple of 8
    int padding = 8 - bitstream.length() % 8;
    for (int i = 0; i < padding; ++i) {
        bitstream += "0";
    }

    // Write the compressed data to file
    std::ofstream outFile(outputFilename, std::ios::binary);

    // Serialize the Huffman tree
    serializeTree(root, outFile);

    // Write the padding size
    outFile.put(static_cast<char>(padding));

    // Write the bitstream as bytes
    for (size_t i = 0; i < bitstream.size(); i += 8) {
        std::bitset<8> byte(bitstream.substr(i, 8));
        outFile.put(static_cast<char>(byte.to_ulong()));
    }

    std::cout << "Compression completed!" << std::endl;
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
    std::string inputXML = "sample.xml";
    std::string compressedFile = "compressed_file_test.huff";
    std::string outputXML = "output.xml";

    // Compress the XML file into a compressed file
    compressXML(inputXML, compressedFile);

    // Decompress the file and output to a new XML file
    decompressXML(compressedFile, outputXML);

    return 0;
}
