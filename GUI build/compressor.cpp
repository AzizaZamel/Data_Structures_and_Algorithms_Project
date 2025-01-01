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

/*int main() {
    std::string inputXML = "sample.xml";
    std::string compressedFile = "compressed_file_test.huff";

    // Compress the XML file into a compressed file
    compressXML(inputXML, compressedFile);

    return 0;
}
*/
