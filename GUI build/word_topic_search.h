/*
 ===================================================================================================
 Name        : word_topic_search.h
 Author      : Doaa Said
 Description :header file declares the post structure, along with function
 prototypes retrieving posts, and searching by word or topic.
 It includes necessary libraries for string handling, regular expressions,
 and XML parsing.
 Date        : 18/12/2024
 ===================================================================================================
 */
#ifndef WORD_TOPIC_SEARCH_H_  // Include guard to prevent multiple inclusions of this header file
#define WORD_TOPIC_SEARCH_H_

#include <iostream>  // Include iostream for input/output operations
#include <string>    // Include string library for string handling
#include <vector>    // Include vector library for dynamic array handling
#include <regex>     // Include regex library for regular expression handling
#include "xml_helper.hpp"  // Include external header for XML helper functions

using namespace std;

// Define the structure for a Post
struct post {
    string body;                  // Post body content (a string)
    vector<string> topics; // List of topics associated with the post (vector of strings)
};

// Function to get all posts from the XML file
vector<post> get_posts(const string &filename); // Parses the XML file and returns a vector of posts

// Function to search for a word in the posts and return matching posts
 std::string search_by_word( const  string&filename, const std::string &word); // Searches posts for a specific word

// Function to search for posts by a specific topic
std::string search_by_topic( const string &filename, const std::string & topic); // Searches posts by topic

#endif /* WORD_TOPIC_SEARCH_H_ */  // End of the include guard
