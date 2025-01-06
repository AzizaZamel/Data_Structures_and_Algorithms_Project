/*
 ===================================================================================================
 Name        : Minify.cpp
 Author      : Doaa Said
 Description : To minify XML,the process involves removing unnecessary spaces,
 line breaks, and comments while retaining the essential structure.
 Date        : 28/11/2024
 ===================================================================================================
 */

#include "Minifing.h"

using namespace std;


/*int main(void) {
	//for testing only
	string filename = "samplu.xml";
	string minified = minify_xml_file(filename, "output_file.xml");
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
*/
void minify_string(string str, string &output) {
	//cout << str << "\n"; // Output the original content for debugging or observation
	//string output;  // String to store the minified result
	bool new_line_come = false; // Boolean flag to track if a newline was encountered
	bool openTag_first_come = false; //Boolean flag to check the first open tag encountered
	bool in_comment = false; //Boolean flag to indicate that text is a comment
	bool in_declaration = false; //Boolean flag to indicate that text is a declaration
	bool spaces_after_closing_tag = false; //Boolean flag to indicate spaces after'>' before comment
	bool conseq_spaces = false; // Boolean flag to keep track of consecutive spaces outside tags
	// Loop through the file contents character by character

	for (unsigned int i = 0; i < str.length(); i++) {

		if (str[i] != '\n') {
			// If the character is not a newline
			// Check if the current character is the start of a declaration
			if (str[i] == '<' && str[i + 1] == '?') {
				in_declaration = true;  // Mark the start of a declaration
				;
			}
			// Check if the current character is the end of a declaration
			else if (str[i] == '>' && str[i - 1] == '?' && in_declaration) {
				in_declaration = false;  // Mark the end of a declaration
			}

			// Skip characters if inside a declaration or if the current character is a closing tag of a declaration
			if (in_declaration || (str[i] == '>' && str[i - 1] == '?')) {
				continue;
			}

			// Check if the current character is the start of a comment
			if (str[i] == '<' && str[i + 1] == '!') {
				in_comment = true;  // Mark the start of a comment
			}

			// Check if the current character is the end of a comment
			else if (str[i] == '>' && str[i - 1] == '-' && in_comment) {
				in_comment = false;  // Mark the end of a comment
			}

			// Skip characters if inside a comment or if the current character is a closing tag of a comment
			if (in_comment || (str[i] == '>' && str[i - 1] == '-')) {
				continue;
			}

			if (str[i] == ' ' && str[i - 1] == '>'
					&& !spaces_after_closing_tag) {
				/* Check if the current character is a space,
				 * the previous character is a closing tag '>',
				 * and we haven't already encountered spaces after a closing tag
				 */
				spaces_after_closing_tag = true; // Set flag to indicate spaces after the closing tag
			} else if (spaces_after_closing_tag && str[i] != ' ') {
				/* If we are currently in the state where there are spaces after the closing tag,
				 * but the current character is not a space,
				 * reset the flag
				 */
				spaces_after_closing_tag = false; // No longer encountering spaces after the tag
			}

			/* Check for consecutive spaces in text outside tags that come after the original text
			 * or consecutive spaces inside text that
			 * or a space before the next opening tag '<'
			 */
			if(spaces_after_closing_tag||str[i]=='\t'||str[i]=='\r'){
				continue;
			}

			if (((str[i] == ' ' && str[i + 1] == ' ')
					|| (str[i] == ' ' && str[i + 1] == '<'))
					&& !conseq_spaces) {
				// If two consecutive spaces are found OR if the space is just before an opening tag '<',
				// set the 'conseq_spaces' flag to true to indicate consecutive spaces.
				conseq_spaces = true;
			}
			// If we encounter a non-space character after consecutive spaces, reset the flag
			else if (conseq_spaces && str[i] != ' ') {
				// If we were skipping spaces and now encounter a non-space character,
				// reset the 'conseq_spaces' flag to false, indicating no longer in a space-block.
				conseq_spaces = false;
			}

			// If we are currently in the state of handling consecutive spaces, skip the current space.
			if (conseq_spaces) {
				continue; // Skip this space, it's part of the consecutive space block
			}

			/* Skip spaces that follow a newline(indentation)to avoid extra space
			 * or skip spaces before the first open tag if found
			 */
			if ((str[i] == ' ' && new_line_come)
					|| ((!openTag_first_come && str[i] == ' '))) {

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

}

// Function to minify the content of the file by removing excess whitespace
string minify_xml_file(const string &input_filename,
		const string &output_filename) {

	string output;

	// Get the content of the input file as a string
	string str = fileToString(input_filename);

	// Minify the XML content by removing unnecessary whitespace and comments
	minify_string(str, output);

	// Write the minified content into the output file
	stringToFile( output,output_filename);

	// Return the minified content as a string
	return output;
}
