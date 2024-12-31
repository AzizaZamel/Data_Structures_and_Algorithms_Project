#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <vector>
#include <string>
#include <bitset>
#include <sstream>

using namespace std;


struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Comparator for priority queue
struct Compare {
    bool operator()(Node* left, Node* right) {
        return (left->freq) > (right->freq);
    }
};


void generateCodes(Node* root, const string& code, map<char, string>& huffmanCodes) {
    if (!root) return;

    // leaaves
    if (!root->left && !root->right) {
        huffmanCodes[root->ch] = code;
    }

    generateCodes(root->left, code + "0", huffmanCodes);
    generateCodes(root->right, code + "1", huffmanCodes);
}

// Build Huffman tree
Node* buildHuffmanTree(map<char, int>& frequencies) {
    priority_queue<Node*, vector<Node*>, Compare> pq; //I added a compare struct to make it min heap

    for (auto& pair : frequencies) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;

        pq.push(merged);
    }

    return pq.top();
}

// Write compressed data to file
<<<<<<< HEAD
void writeCompressedFile(const string& input,const map<char, string>& huffmanCodes,const string& outputFile) {
=======
void writeCompressedFile(string& input, map<char, string>& huffmanCodes, string& outputFile) {
>>>>>>> 07b4d226e285f17d00fa4faed3dbeb8e3fc06d7e
    ofstream outFile(outputFile, ios::binary); // making a bin file (compressed)
    if (!outFile) {
        cerr << "Error opening file for writing: " << outputFile << endl;
        return;
    }

    // Encode input data
    string encodedString = "";
    for (char ch : input) {
        encodedString += huffmanCodes.at(ch);
    }

    // Convert to binary and write
    stringstream buffer;
    // Writing byte by byte 
    for (int i = 0 ; i < encodedString.size(); i += 8) {
        bitset<8> byte(encodedString.substr(i , 8));
        buffer.put(static_cast<unsigned char>(byte.to_ulong())); 
        /* regarding the line above , honestly chatgpt suggested 
        this static cast to protect from bad alloc*/
    }

    outFile.write(buffer.str().c_str(), buffer.str().size());
    outFile.close();
}
// File to string stream function
<<<<<<< HEAD
string filetoss(const string& filename) {
=======
string filetoss(string& filename) {
>>>>>>> 07b4d226e285f17d00fa4faed3dbeb8e3fc06d7e
    ifstream file(filename); // Open the file
    if (!file.is_open()) {
        cerr << "Can't open file";
        
    }

    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();  
}
// Main compression function
<<<<<<< HEAD
void compressXML(const string& inputFile,const string& outputFile) {
=======
void compressXML(string& inputFile, string& outputFile) {
>>>>>>> 07b4d226e285f17d00fa4faed3dbeb8e3fc06d7e
   
    // Read input file content
    string input = filetoss(inputFile);
   

    // Calculate character frequencies
    map<char, int> frequencies;
    for (char ch : input) {
        frequencies[ch]++;
    }

    // Build Huffman Tree
    Node* root = buildHuffmanTree(frequencies);

    // Generate Huffman Codes
    map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    // Compress the input file
    writeCompressedFile(input, huffmanCodes, outputFile);

    cout << "File compressed successfully to " << outputFile << endl;
}
