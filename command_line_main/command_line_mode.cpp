/*
 ===================================================================================================
 Name        : command_line_mode.cpp
 Author      : Doaa Said
 Description : program is designed to handle various operations related to XML files and user
 data. It processes command-line arguments to determine which operation to perform
 Date        : 31/12/2024
 ===================================================================================================
 */
#include<iostream>  // Standard input/output stream
using namespace std;
#include "graph.hpp"  // Header for graph-related operations
#include "prettify.hpp"  // Header for pretty printing XML
#include "xml2json.hpp"  // Header for converting XML to JSON
#include "Minifing.h"  // Header for minifying XML
#include "most_active.hpp"  // Header for most active user functionality
#include "command_line_mode.h"  // Header for command line processing
#include "word_topic_search.h"  // Header for searching by word or topic
#include "xml_checker.hpp"  // Header for XML consistency checking
#include "suggest.hpp"  // Header for suggestions
#include "compressor.h"  // Header for compressing XML
#include "mutual_followers.hpp"  // Header for mutual followers functionality
#include "xml_util.hpp"  // Header for XML utility functions
#include "xml_parser.hpp"  // Header for XML parsing
#include "xml_node.hpp"  // Header for XML node handling
#include "utils.hpp"  // Utility functions header
#include "user.hpp"  // Header for user-related functionality
#include "most_influencer.hpp"  // Header for getting most followed user details

// Main function - entry point of the program
int main(int argc, char *argv[]) {
	try {
		parseCommand(argc, argv);  // Parse the command-line arguments
	} catch (const std::exception &e) {
		// Catch any exceptions and print the error message
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;  // Return failure if an exception is caught
	}
	return EXIT_SUCCESS;  // Return success if no exception occurs
}

// Function to parse the command-line arguments and execute respective actions
void parseCommand(int argc, char *argv[]) {

	string command = argv[1];  // The first argument specifies the command
	int id;
	string word, topic;
	std::string input_File, output_File;
	bool fix_Errors = false;  // Flag to fix errors in XML
	vector<int> user_ids;  // Vector to store user IDs

	// Loop over the arguments to process each one
	for (int i = 2; i < argc; ++i) {
		std::string arg = argv[i];

		// Check if the argument is for input file
		if (arg == "-i") {
			if (++i < argc) {
				input_File = argv[i];  // Set the input file
			} else {
				throw std::invalid_argument("Error: Missing input file.\n"); // Handle missing file error
			}
		}
		// Check if the argument is for output file
		else if (arg == "-o") {
			if (++i < argc) {
				output_File = argv[i];  // Set the output file
			} else {
				throw std::invalid_argument("Error: Missing output file.\n"); // Handle missing file error
			}
		}
		// Check if the argument is to fix XML errors
		else if (arg == "-f") {
			fix_Errors = true;  // Set the flag to fix errors
		}
		// Check if the argument is for user IDs
		else if (arg == "-ids") {
			if (++i < argc) {
				std::stringstream ss(argv[i]);
				std::string idStr;
				// Parse the comma-separated user IDs
				while (std::getline(ss, idStr, ',')) {
					try {
						user_ids.push_back(std::stoi(idStr)); // Convert string to integer and add to user_ids vector
					} catch (const std::invalid_argument&) {
						throw std::invalid_argument(
								"Error: Invalid user ID in -ids argument.\n"); // Handle invalid ID error
					}
				}
			} else {
				throw std::invalid_argument("Error: Missing IDs after -ids.\n"); // Handle missing user IDs error
			}
		}
		// Check if the argument is for a single user ID
		else if (arg == "-id") {
			if (++i < argc) {
				id = std::stoi(argv[i]);  // Set the single user ID
			} else {
				throw std::invalid_argument("Error: Missing ID after -id.\n"); // Handle missing ID error
			}
		}
		// Check if the argument is for a word
		else if (arg == "-w") {
			if (++i < argc) {
				word = argv[i];  // Set the word to search for
			} else {
				throw std::invalid_argument("Error: Missing word after -w.\n"); // Handle missing word error
			}
		}
		// Check if the argument is for a topic
		else if (arg == "-t") {
			if (++i < argc) {
				topic = argv[i];  // Set the topic to search for
			} else {
				throw std::invalid_argument("Error: Missing topic after -t.\n"); // Handle missing topic error
			}
		}
		// Handle unknown arguments
		else {
			throw std::invalid_argument(
					"Error: Unknown argument: " + arg + "\n"); // Handle unknown argument error
		}
	}

	// Perform actions based on the parsed command
	if (command == "format") {
		prettifyXML(input_File, output_File);  // Prettify the XML file
	} else if (command == "json") {
		convertXmlToJson(input_File, output_File);  // Convert XML to JSON
	} else if (command == "mini") {
		minify_xml_file(input_File, output_File);  // Minify the XML file
	} else if (command == "compress") {
		compressXML(input_File, output_File);  // Compress the XML file
	} else if (command == "decompress") {
		decompressXML(input_File, output_File);  // Decompress the XML file
	} else if (command == "verify") {
		checkConsistency_Xml_file(input_File);  // Verify XML consistency
		if (fix_Errors) {
			fix_XML_File(input_File, output_File); // Fix XML if errors are found
		}
	} else if (command == "draw") {

		generate_graph(input_File, output_File);  // Generate JPG
	} else if (command == "most_active") {
		most_active(input_File);  // Get the most active user
	} else if (command == "most_influencer") {
		cout << getMostFollowedUserDetails(input_File); // Get the most followed user details
	} else if (command == "mutual") {
		cout << get_mutual_followers_string(user_ids, input_File); // Get mutual followers for specified user IDs
	} else if (command == "suggest") {
		printS(print_suggested(input_File, id), id); // Print suggestions based on the user ID
	} else if (command == "search") {
		// Search based on the given word or topic
		if (word.empty()) {
			cout << search_by_topic(input_File, topic);  // Search by topic
		} else {
			cout << search_by_word(input_File, word);  // Search by word
		}
	}
}

// Function to fix XML file if errors are found
void fix_XML_File(const std::string &input_file,
		const std::string &output_file) {
	string xmlContent = fileToString(input_file); // Read the XML content from the input file
	XmlChecker checker;  // Create an XmlChecker object
	checker.fixXml(xmlContent, output_file);  // Fix XML and save to output file
}

// Function to check XML file for consistency
void checkConsistency_Xml_file(const std::string &input_file) {
	string xmlContent = fileToString(input_file); // Read the XML content from the input file
	XmlChecker checker;  // Create an XmlChecker object
	checker.checkConsistency(xmlContent);  // Check XML consistency
}
