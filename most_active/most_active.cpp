/********************  Author:Toqa Gamal Mohamed   *****************/
/********************  Project:DSA_project_part_2  *****************/
/********************  File:most_active.cpp        *****************/
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include "most_active.hpp" 
using namespace std;

void helper_function(const string& xmlfile) {
    string inputfilename = xmlfile ;
    // Function to generate a graph from an XML file.
    Graph* v = generateGraph(inputfilename);
    //Call main function.
    most_active_user(v);
}


string most_active_user(Graph* g) {
    unordered_map<int, string> users = g->getUsers(); // Get users from the graph
    unordered_map<int, int> in_degree; // To count in-degrees for each user
    unordered_map<int, int> out_degree; // To count out-degrees for each user

    // Iterate through the graph's adjacency list to count in-degrees and out-degrees
    for (const auto& pair : g->getMap()) {
        int user_id = pair.first;
        const list<int>& connections = pair.second;

        // Count the number of out-connections (out-degree)
        out_degree[user_id] = connections.size();

        // Count the number of in-connections (in-degree)
        for (int follower_id : connections) {
            in_degree[follower_id]++; // Increment in-degree for each follower
        }
    }

    // Now, find the users with the maximum sum of in-degree + out-degree
  
    // Store the most active users and their ID/name as pairs
    vector<pair<int, string>> most_active_users; // Store user ID and name
    int max_degree_sum = -1;

    // Iterate through the users to find the most active ones
    for (const auto& entry : users) {
        int user_id = entry.first;
        const string& user_name = entry.second;

        // Calculate the degree sum (in-degree + out-degree)
        int degree_sum = in_degree[user_id] + out_degree[user_id];

        if (degree_sum > max_degree_sum) {
            // Found a new maximum degree sum
            max_degree_sum = degree_sum;
            most_active_users.clear(); // Clear previous users
            most_active_users.push_back( make_pair(user_id, user_name) ) ; // Add the new most active user
        }
        else if (degree_sum == max_degree_sum) {
            // Add this user to the list if they have the same degree sum
            most_active_users.push_back( make_pair(user_id, user_name) );
        }
    }
    cout << endl;
    // Construct the result string
    string result = " Most Active Users (In + Out Degree): ";
    for (const auto& user : most_active_users) {
        result += " ID: " + to_string( user.first ) + " , Name: " + user.second + " ;";
    }

    result += " Maximum Degree Sum: " + to_string( max_degree_sum );

    cout << result;
    cout << endl; 

    return result;
}

// Test the function 

int main() {
    Graph* t = generateGraph("already_prettified.txt");
    t->printUsers();

    // Find and print the most active user
    most_active_user(t);

    delete t; // Clean up the dynamically allocated graph
    return 0;
}
