/*
 * word&topic_search.h
 *  Created on: Dec 28, 2024
 *   Author: Doaa
 *
 *  Description:
 *  This header file defines the structure and functions used for parsing an XML file
 *  and searching within user posts. It includes:
 *  - A `Post` struct to represent individual posts with their body and associated topics.
 *  - Declarations of functions for retrieving posts and performing searches by word or topic.
 *
 *  Dependencies include XML utility and parsing libraries for handling the XML file.
 */

#ifndef WORD_TOPIC_SEARCH_H_
#define WORD_TOPIC_SEARCH_H_

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "xml_util.hpp"
#include "xml_node.hpp"
#include "xml_parser.hpp"

using namespace std;

// Define the structure for a Post
struct Post {
    std::string body;
    std::vector<std::string> topics;
};

// Function to get all posts from the XML file
vector<Post> get_posts(string filename);

// Function to search for a word in the posts and return matching posts
string search_by_word(string filename, string word);

// Function to search for posts by a specific topic
string search_by_topic(string filename, string topic);

#endif /* WORD_TOPIC_SEARCH_H_ */
