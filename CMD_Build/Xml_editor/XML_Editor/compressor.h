#ifndef compressor
#define compressor

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <tuple>
#include <sstream>
using namespace std;
vector<vector<char>> readFileIntoBlocks(const std::string& fileName);
vector<tuple<int, int, char>> lz77Compress(const vector<char>& input);
string serializeLZ77(const vector<tuple<int, int, char>>& compressedData);
// Function to write serialized LZ77 data to a file
void writeLZ77ToFile(const string& filename, const string& data);
// Function to deserialize LZ77 compressed data from a string
vector<tuple<int, int, char>> deserializeLZ77(const string& data);
// LZ77 decompression function
string lz77Decompress(const vector<tuple<int, int, char>>& compressedData);
// Function to read a file's content into a string
string readFileToString(const string& filename);
void compressXML(const string& inputfilename , const string& outputfilename);
void decompressXMLfile(const string& inputfilename , const string& outputfilename);
#endif
