/********************  Author:Toqa Gamal Mohamed   *****************/
/********************  Project:DSA_project_part_2  *****************/
/********************  File:most_active.cpp        *****************/
#include <iostream>
#include <unordered_map>
#include <string>
#include "most_active.hpp" 
using namespace std;

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

    // Now, find the user with the maximum sum of in-degree + out-degree
    int most_active_id = -1;
    string most_active_name;
    int max_degree_sum = -1;

    for (const auto& entry : users) {
        int user_id = entry.first;
        const string& user_name = entry.second;

        // Get the in-degree and out-degree
        int degree_sum = in_degree[user_id] + out_degree[user_id];

        if (degree_sum > max_degree_sum) {
            max_degree_sum = degree_sum;
            most_active_id = user_id;
            most_active_name = user_name;
        }
    }

    // Return the most active user's name and ID based on in-degree + out-degree sum
    string result = "Most Active User (In + Out Degree): " + most_active_name + ", ID: " + to_string(most_active_id);

    return result;
}

// Test the function 

int main() {
    Graph* t = generateGraph("already_prettified.txt");
    t->printUsers();


    // Find and print the most active user
    string active_user_info = most_active_user(t);
    cout << active_user_info << endl;

    delete t; // Clean up the dynamically allocated graph
    return 0;
}
