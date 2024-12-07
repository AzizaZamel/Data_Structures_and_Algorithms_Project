/*
 * Minifing.cpp
 * Created on: Nov 28, 2024
 *  Author: Doaa Said
 */

#include<iostream>
#include<fstream>
#include <sstream>
#include <ostream>
#include <string>

using namespace std;

// Function to read the contents of a file and return it as a string.
string convert_file_to_string(const string &filename) {
	ifstream file(filename); // Open the file
	if (!file.is_open()) {   // Check if the file is successfully opened
		fprintf(stderr, "can't open file"); // Print error if file can't be opened
	}

	stringstream buffer;  // Create a stringstream to hold the file contents
	buffer << file.rdbuf(); // Read the entire file into the buffer
	file.close();  // Close the file after reading
	return buffer.str();  // Return the contents of the file as a string
}
void write_string_into_file(const string &output_filename, string output) {

	ofstream outFile(output_filename); // Open the output file to write the minified content

	// Check if the output file is open
	if (outFile.is_open()) {
		// Write the minified content to the output file
		outFile << output;
	} else {
		fprintf(stderr, "can't open file"); // Print error if file can't be opened
	}
}

// Function to minify the content of the file by removing excess whitespace
string minify(const string &input_filename, const string &output_filename) {
	string str =convert_file_to_string(input_filename);  // Get the file content as a string

	cout << str << "\n"; // Output the original content for debugging or observation
	string output;  // String to store the minified result
	bool new_line_come = false; // Boolean flag to track if a newline was encountered
	bool openTag_first_come = false; //Boolean flag to check the first open tag encountered
	// Loop through the file contents character by character
	for (unsigned int i = 0; i < str.length(); i++) {

		if (str[i] != '\n') {  // If the character is not a newline

			if ((str[i] == ' ' && new_line_come)
					|| (!openTag_first_come && str[i] == ' ')) {
				/* Skip spaces that follow a newline(indentation)to avoid extra space
				 * or skip spaces before the first open tag if found
				 */
				continue;
			} else {
				output += str[i]; // Otherwise, add the character to the output string
				if (str[i] == '<' && !openTag_first_come) {
					//if first "<" found
					openTag_first_come = 1; //set the flag to indicate the first open tag

				}
			}

			//end of indentation and another char come or there isn't indentation after newline
			if (str[i] != ' ' && new_line_come) {
				new_line_come = false; // Reset the flag to indicate no newline after space
			}

		} else {
			new_line_come = true; // Set flag when encountering a newline character
		}
	}

	write_string_into_file(output_filename, output);

	return output;  // Return the minified content as a string
}
int main(void) {
	//for testing only
	string filename = "sample.xml";
	string minified = minify(filename, "output_file.xml");
	std::ifstream inFile("output_file.xml"); // Open the file for reading
	ofstream outFile2("output_file.txt");

	// Check if the file is open
	if (outFile2.is_open()) {
		// Write a string to the file
		outFile2 << minified;
	}
	if (inFile.is_open()) {
		std::string line;

		while (std::getline(inFile, line)) {
			std::cout << line << "\n"; // Output the content to the console
		}

		inFile.close(); // Close the file
	} else {
		std::cerr << "Unable to open the file for reading.\n";
	}
	std::ifstream inFile2("output_file.txt");
	if (inFile2.is_open()) {
		std::string line2;

		while (std::getline(inFile2, line2)) {
			std::cout << line2 << "\n"; // Output the content to the console
		}

		inFile2.close(); //
	}
	return 0;

}


