/*
 * word&topic_search.cpp
 *  Created on: Dec 19, 2024
 *   Author: Doaa
 *
 *  Description:
 *  This file implements functionalities to parse an XML file and extract user posts.
 *  It provides:
 *  - `get_posts`: Retrieves all posts along with their topics from an XML file.
 *  - `search_by_word`: Searches for posts containing a specific word in their body.
 *  - `search_by_topic`: Searches for posts related to a specific topic.
 *
 *  It leverages an XML parsing library to read and traverse the XML file and uses regex
 *  for precise word matching.
 */

#include "word_topic_search.h"

// Function to retrieve all posts and their associated topics from the given XML file.
std::vector<Post> get_posts(string filename) {
    // Create an instance of the XML parser
    xml_parser_lib::xml_parser parser;

    // Parse the XML file into a tree structure
    auto root = parser.parse_from_file(filename);

    std::vector<Post> posts;

    // Traverse the root element to find user nodes
    for (const auto& user_node : root->get_children()) {
        // Ensure the node is a user node
        if (user_node->get_name() == "user") {
            // Find the "posts" child node of the user
            for (const auto& post_node : user_node->get_children()) {
                if (post_node->get_name() == "posts") {
                    // Iterate through each post
                    for (const auto& individual_post_node : post_node->get_children()) {
                        if (individual_post_node->get_name() == "post") {
                            // Extract the post body
                            std::string body;
                            for (const auto& body_node : individual_post_node->get_children()) {
                                if (body_node->get_name() == "body") {
                                    body = body_node->get_text();
                                }
                            }

                            // Extract the topics
                            std::vector<std::string> topics;
                            for (const auto& topics_node : individual_post_node->get_children()) {
                                if (topics_node->get_name() == "topics") {
                                    for (const auto& topic_node : topics_node->get_children()) {
                                        if (topic_node->get_name() == "topic") {
                                            topics.push_back(topic_node->get_text());
                                        }
                                    }
                                }
                            }

                            // Create a Post object and add it to the vector
                            posts.push_back({body, topics});
                        }
                    }
                }
            }
        }
    }

    return posts;
}

// Function to search posts containing a specific word in their body.
string search_by_word(string filename, string word) {
    string posts;
    vector<Post> p;
    p = get_posts(filename);

    string regexPattern = "\\b" + word + "\\b"; // Match the word as a whole word
    regex wordRegex(regexPattern);

    for (Post a : p) {
        // Check if the word exists in the post body using regex
        if (regex_search(a.body, wordRegex)) {
            posts.append(a.body + "\n"); // Append matching posts to the result
        }
    }

    return posts;
}

// Function to search posts related to a specific topic.
string search_by_topic(string filename, string topic) {
    string posts;
    vector<Post> p;
    p = get_posts(filename);

    for (Post a : p) {
        for (string b : a.topics) {
            if (topic == b) { // Check if the topic matches
                posts.append(a.body + "\n"); // Append matching posts to the result
            }
        }
    }

    return posts;
}
