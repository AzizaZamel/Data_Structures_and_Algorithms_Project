#ifndef MOST_INFLUENCER_HPP
#define MOST_INFLUENCER_HPP

#include "graph.hpp"
#include <string>
#include <utility> // For std::pair

class GraphHelpers {
public:
    // Function to find the user with the most followers in the graph.
    static std::pair<int, std::string> getUserWithMostFollowers(Graph& g);
};

// Function to get the details of the user with the most followers.
// This function creates the graph and uses GraphHelpers.
std::string getMostFollowedUserDetails(const std::string& filename);

#endif // MOST_INFLUENCER_HPP
