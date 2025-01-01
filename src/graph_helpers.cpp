#include "graph_helpers.hpp"
#include <utility> // For std::pair
#include "graph.hpp"
#include "xml_helper.hpp"
#include <iostream>
#include <string>
#include <string>

using namespace std;


pair<int, string> GraphHelpers::getUserWithMostFollowers(Graph& g) {
    const auto& adjacencyMap = g.getMap(); // The graph structure: user -> list of followers
    const auto& users = g.getUsers();      // User ID to name mapping

    int maxFollowers = 0;                  // Variable to track the maximum number of followers
    int userIdWithMostFollowers = -1;      // ID of the user with the most followers

    // Iterate over the adjacency map
    for (const auto& pair : adjacencyMap) {
        int currentFollowers = pair.second.size(); // Size of the follower list
        if (currentFollowers > maxFollowers) {
            maxFollowers = currentFollowers;
            userIdWithMostFollowers = pair.first; // Update the user with the most followers
        }
    }

    // Return the user ID and name
    if (userIdWithMostFollowers != -1 && users.count(userIdWithMostFollowers)) {
        return {userIdWithMostFollowers, users.at(userIdWithMostFollowers)};
    } else {
        return {-1, "No users found"};
    }
}

string getMostFollowedUserDetails(const string& filename) {
    // Generate the graph from the input file
    Graph* g = generateGraph(filename);

    // Find the user with the most followers
    pair<int, string> mostFollowedUser = GraphHelpers::getUserWithMostFollowers(*g);

    // Prepare the result string
    string result;
    if (mostFollowedUser.first != -1) {
        result = "User with the most followers:\n";
        result += "ID: " + to_string(mostFollowedUser.first) + ", Name: " + mostFollowedUser.second + "\n";
    } else {
        result = "No user with followers found.\n";
    }

    // Clean up
    delete g;

    return result;
}


