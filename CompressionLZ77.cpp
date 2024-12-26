#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <deque>

using namespace std;

const size_t BLOCK_SIZE = 32 * 1024; // 32KB

 //function te read input json or xml file

string readInputFile(string filename) {
    cout << "Reading input file: " << filename << endl;
    // Code to read input file
    ifstream inputFile(filename);   // Open the file
    if (inputFile.is_open()) {      // Check if the file is open
        string line;                // Variable to store each line (convert file to string)
        while (getline(inputFile, line)) {      // Read each line from the file
            cout << line << endl;               // Output the line to the console
        }
        inputFile.close();
        return line;
    } 
    
    return  "Unable to open file: " + filename;
}

// function to devide input file into blocks of 32KB
//store the blocks in a vector for efficient memory management and fast random access

vector<vector<char>> readFileIntoBlocks(const string& fileName) {

    ifstream inputFile(fileName, ios::binary); // Open file in binary mode
    if (!inputFile.is_open()) {
        throw runtime_error("Error: Could not open the file!"); // Throw an exception if file cannot be opened
    }
  
    // Vector to hold the file blocks, vector of vectors of characters to ease processing of each block as a vector of chars not a string
    vector<vector<char>> blocks; 
    char buffer[BLOCK_SIZE];    // fixed sized array to read a block of data (temporary buffer)
    size_t totalSize = 0;  // To track the total size of the data

    while (inputFile.read(buffer, BLOCK_SIZE) || inputFile.gcount() > 0) { // Read a block of data or until the end of file
        // Read a block and store it in the vector

        size_t bytesRead = inputFile.gcount();
        totalSize += bytesRead;

        blocks.emplace_back(buffer, buffer + inputFile.gcount()); 

        // Print the current block (for demonstration, print as hex values)
        cout << "Block size: " << bytesRead << " bytes\n";
        cout << "Block data (in hex): ";
        for (size_t i = 0; i < bytesRead; ++i) {
            cout << hex << static_cast<int>(buffer[i]) << " ";
        }
        cout << "\n\n";
    }

    inputFile.close();

      // Print the total size of the data
    cout << "Total size of data: " << dec << totalSize << " bytes\n";

    return blocks; // Return the vector of blocks
}

//LZ77 is a lossless compression algorithm that replaces repeated sequences of data with references to a single copy

void lz77(vector<vector<char>> blocks) {
    /*Sliding Window:
    A sliding window keeps track of previously seen data (history). It’s divided into two parts: search buffer and look-ahead buffer.
    Search buffer: Contains previously seen data.
    Look-ahead buffer: Contains data that is yet to be processed.*/

    int windowSize = 32 * 1024; // 32KB
    int lookAheadBufferSize = 256; // 256B, can be changed
    int searchBufferSize = windowSize - lookAheadBufferSize; // 32KB - 256B = 31.75KB

    // Initialize the sliding window, using a double ended queue (deque) for best time complexity in insertion and deletion
    deque<char> slidingWindow;

    



 

}



int main() {
    
    string filename;
    cout << "Enter the name of the file to read: ";
    cin >> filename;
    /*
    string content = readInputFile(filename);
    cout << "Content of the file: " << content << endl;
    return 0;
    */
    try {
        // Call the function and get the vector of blocks
        vector<vector<char>> blocks = readFileIntoBlocks(filename);

        // Print the number of blocks
        cout << "Number of blocks: " << blocks.size() << endl;
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
  
