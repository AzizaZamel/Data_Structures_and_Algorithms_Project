/*
 ===================================================================================================
 Name        : word_topic_search.cpp
 Author      : Doaa Said
 Description : defines the post structure and declares functions for retrieving
 posts, and searching posts by word or topic. It includes necessary libraries
 for string manipulation, regular expressions,and XML handling.
 Date        : 18/12/2024
 ===================================================================================================
 */

#include "word_topic_search.h" // Include the header file for word_topic_search functions

using namespace std;

#include <stdexcept> // Include for throwing exceptions
#include <iostream>

// Function to parse XML and return a vector of posts
std::vector<post> get_posts(const string &filename) {
    std::vector<std::string> parsedXML = parser(filename); // Parse the XML using an external 'parser' function
    std::vector<post> posts; // Vector to store the posts extracted from XML

    // Iterate over the parsed XML elements
    for (size_t i = 0; i < parsedXML.size(); i++) {
        // Check if we are at the start of a <post> element
        if (parsedXML[i] == "<post>") {
            post a; // Create a new post object
            // Iterate through the XML until we find the end of the post element
            while (i < parsedXML.size() && parsedXML[i] != "</post>") {
                i++; // Move to the next XML element
                // If it's a <body> element, assign the next string to the body of the post
                if (parsedXML[i] == "<body>") {
                    if (i + 1 < parsedXML.size()) {
                        a.body = parsedXML[++i]; // Assign body content to the post
                    } else {
                        throw std::runtime_error(
                            "Malformed XML: Missing <body> content."); // Throw error if body content is missing
                    }
                }
                // If it's a <topics> element, process the topics inside the post
                else if (parsedXML[i] == "<topics>") {
                    // Process all topics inside <topics>
                    while (i < parsedXML.size() && parsedXML[i] != "</topics>") {
                        i++; // Move to the next element
                        if (parsedXML[i] == "<topic>") {
                            if (i + 1 < parsedXML.size()) {
                                a.topics.push_back(parsedXML[++i]); // Add each topic to the post
                            } else {
                                throw std::runtime_error(
                                    "Malformed XML: Missing <topic> content."); // Throw error if topic content is missing
                            }
                        }
                    }
                }
            }
            posts.push_back(a); // Add the fully created post to the posts vector
        }
    }
    return posts; // Return the vector of posts
}

// Function to search for posts containing a specific word
string search_by_word( const string &filename, const std::string &word) {
    std::vector<post> posts = get_posts(filename); // Get all posts from the XML file
    std::string result; // String to store the result of the search

    // Regular expression to match the word as a whole word
    std::string regexPattern = "\\b" + word + "\\b"; // Word boundaries for exact match
    std::regex wordRegex(regexPattern); 

    // Iterate through each post
    for (const post &a : posts) {
        // Check if the word is found in the post body using regex search
        if (std::regex_search(a.body, wordRegex)) {
            result.append(a.body + "\n"); // Append the post body to the result
        }
    }

    // Return the result or a message if no posts were found containing the word
    return result.empty() ?
               "No posts found containing the word: " + word : result;
}

// Function to search for posts by topic
std::string search_by_topic( const string  &filename, const std::string & topic) {
    std::vector<post> posts = get_posts(filename); // Get all posts from the XML file
    std::string result; // String to store the result of the search

    // Iterate through each post
    for (const post &a : posts) {
        // Iterate through the topics of the post
        for (const std::string &t : a.topics) {
            // If a topic matches, add the body of the post to the result
            if (t == topic) {
                result.append(a.body + "\n");
                break; // Break the loop as we've found a match, no need to check further topics
            }
        }
    }

    // Return the result or a message if no posts were found under the topic
    return result.empty() ? "No posts found under the topic: " + topic : result;
}

// Main function to test the search functions
/*int main(void) {
 vector<string>a;

 // Create a vector of posts
 // vector<post> v;
 v = get_posts("sample.xml");
 for (unsigned int i = 0; i < v.size(); i++) { // Iterate through posts
 cout << "post " << i << "\n"; // Print post number
 cout << v[i].body << "\n"; // Print post body
 for (string a : v[i].topics) { // Iterate through post topics
 cout << a << "  "; // Print each topic
 }
 }
 */
// Search for posts with the topic "Finance" in the "sample.xml" file and print the result
//  cout << search_by_topic("sample.xml", "finance");
/*}*/

