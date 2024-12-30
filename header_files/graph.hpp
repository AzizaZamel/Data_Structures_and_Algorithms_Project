#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class Graph {
private:
    // Adjacency list representation
    unordered_map<int, list<int>> m;

public:

    // Add a vertex to the graph
    void addVertex(int u);

    // Add an edge from startVertex to endVertex
    void addEdge(int startVertex, int endVertex);

    // return the map
    unordered_map<int, list<int>> getMap();

    // Generate a DOT file from the graph
    string generateDotFile();

    // Display the graph (for debugging purposes)
    void printGraph();
};

// Function to generate a graph from an XML file.
Graph* generateGraph(const string& filename);

// Function to generate a JPG image from a DOT file using the Graphviz tool.
void generateJpgFromDot(const string& dotFile, const string& jpgFile);

#endif // GRAPH_H
