#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // For system()
#include <algorithm>
#include <list>
#include <unordered_map>
#include <sstream>
#include "xml_helper.h"
#include "graph.hpp"


void Graph::setUsers(unordered_map<int, string> u) {
    users = u;
}

// Add an edge from startVertex to endVertex
void Graph::addEdge(int startVertex, int endVertex) {
	m[startVertex].push_back(endVertex);
}

// Add a vertex to the graph
void Graph::addVertex(int u) {
	m[u] = list<int>();
}

// return the map
unordered_map<int, list<int>> Graph::getMap() {
    return m;
}

// return the users
unordered_map<int, string> Graph::getUsers() {
    return users;
}

// Display the graph (for debugging purposes)
void Graph::printGraph() {
	for (const auto& pair : m) {
		cout << pair.first << ": ";
		for (const auto& vertex : pair.second) {
			cout << vertex << " ";
		}
		cout << endl;
	}
}

// Display users nams and ids (for debugging purposes)
void Graph::printUsers() {
    for (const auto& pair : users) {
        cout << pair.first << " : " << pair.second << endl;
    }
}

// Generate a DOT file from the graph
string Graph::generateDotFile() {
	// Create an output file stream to write the DOT file
	ofstream dotFile("graph.dot");

	// Check if the file was successfully opened for writing
	if (!dotFile.is_open()) {
		cerr << "Failed to create DOT file.\n";
		return "";
	}

	// Begin the DOT representation of a directed graph
	dotFile << "digraph G {\n";
	// Set layout style to 'dot' for visualizing the graph
	dotFile << "layout=dot;\n";
	// Define a default style for nodes with a filled pink color
	dotFile << "node [ style=filled, color=pink]\n";

	// Iterate over all the vertices and their adjacency lists in the graph
	for (const auto& pair : m) {
		// Check if the current vertex has no outgoing edges
		if (m[pair.first].empty()) {
			// Add a standalone node (vertex with no edges) to the DOT file
			dotFile << pair.first << ";\n";
		}
		else {
			// Add edges for each vertex in the adjacency list
			for (const auto& vertex : pair.second) {
				dotFile << vertex << " -> " << pair.first << ";\n";
			}
		}


	}

	// Close the DOT representation
	dotFile << "}\n";

	// Return the name of the generated DOT file
	return "graph.dot";
}
///
Graph* generateGraph(const string& filename) {
    vector<string> vec = parser(filename);
    Graph* g = new Graph();
    unordered_map<int, string> users;
    int id, f_id;   // Variables to store user ID and follower ID.
    string user_name;
    bool user_flag = false, follower_flag = false, name_flag = false, posts_flag = false;   // Flags to track parsing states.

    for (int i = 0; i < vec.size(); i++) {
        // Start of a user tag.
        if (!vec[i].compare("<user>")) {
            user_flag = true;
        }
        // When inside a user tag, parse the user ID (if not in follower or name/posts sections).
        else if (user_flag && !vec[i].compare("<id>") && !follower_flag && !name_flag && !posts_flag) {
            i++;
            id = stoi(vec[i]);           // Convert the ID token to an integer.
            g->addVertex(id);           // Add the user as a vertex in the graph.
        }
        // Start of a name tag.
        else if (!vec[i].compare("<name>")) {
            name_flag = true;
            i++;
            user_name = vec[i];
        }
        // End of a name tag.
        else if (!vec[i].compare("</name>")) {
            name_flag = false;
        }
        // End of a user tag.
        else if (!vec[i].compare("</user>")) {
            user_flag = false;
            users[id] = user_name;
        }
        // Start of a follower tag.
        else if (!vec[i].compare("<follower>")) {
            follower_flag = true;
        }
        // When inside a follower tag, parse the follower ID.
        else if (follower_flag && !vec[i].compare("<id>")) {
            i++;
            f_id = stoi(vec[i]);        // Convert the follower ID token to an integer.
            g->addEdge(id, f_id);      // Add an edge from the user to the follower in the graph.
        }
        // End of a follower tag.
        else if (!vec[i].compare("</follower>")) {
            follower_flag = false;
        }
        // Start of posts tag.
        else if (!vec[i].compare("<posts>")) {
            posts_flag = true;
        }
        // End of a name or posts tag.
        else if (!vec[i].compare("</posts>")) {
            posts_flag = false;
        }
        
    }
    g->setUsers(users);
    // Return the constructed graph.
    return g;
}


// Function to generate a JPG image from a DOT file using the Graphviz tool.
void generateJpgFromDot(const string& dotFile, const string& jpgFile) {
    // Construct the command to generate a .jpg file using Graphviz's dot tool.
    // The command follows the format: dot -Tjpg inputFile.dot -o outputFile.jpg
    std::string command = "dot -Tjpg "+ dotFile +" -o "+ jpgFile;

    // Execute the command using the system() function.
    // This runs the Graphviz 'dot' command to convert the DOT file into a JPG image.
    int result = system(command.c_str());

    // Check the result of the system() call.
    if (result == 0) {
        cout << "Graph image generated: graph.jpg\n";
    }
    else {
        cerr << "Failed to generate graph image.\n";
    }
}

// test
int main() {

    Graph* g = generateGraph("already_prettified.txt");
    g->printUsers();
    string dotFile = g->generateDotFile();
    generateJpgFromDot(dotFile, "graph.jpg");

    return 0;
}
