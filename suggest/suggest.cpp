#include <iostream>
#include <unordered_map>
#include <list>
#include <set>
#include <unordered_set>
#include "graph.hpp"
#include "suggest.hpp"

using namespace std;

// Function to determine who a given userId follows
vector<int> printWhoUserFollows(const unordered_map<int, list<int>>& m, int userId) {
    //vector to hold the users followed by userId
    vector<int> usersFollowed;

    // Iterate through the map to find who userId follows
    for (const auto& pair : m) {
        // Check if userId is in the list of followers of the current user
        if (find(pair.second.begin(), pair.second.end(), userId) != pair.second.end()) {
            usersFollowed.push_back(pair.first);
        }
    }
    return usersFollowed;
}

void suggest(const unordered_map<int, list<int>>& m, int userId) {
    vector<int> following;      //vector to hold users that userId follows
    vector<int> following_2;    //vector to hold the followers of the followers
    unordered_set<int> result;  //unordered set to avoid duplicates
    following = printWhoUserFollows(m, userId);

    for (int i = 0; i < following.size(); i++) {

        following_2 = printWhoUserFollows(m, following[i]);

        for (int i = 0; i < following_2.size();i++) {
            //if condition to avoid adding the userId or users that userId is already following
            if (following_2[i] != userId && find(following.begin(), following.end(), following_2[i]) == following.end()) {
                
                result.insert(following_2[i]);
            }

        }

    }
    cout << "User " << userId << " suggestions are: " << endl;
    if (result.empty()) {
        cout << "No suggestions available.\n";
    }
    else {
        for (int suggestion : result) {
            cout << suggestion << endl;
        }
    }
}

/*
//usage example:
int main(int argc, char* argv[]) {
    string inputFile = "C:\\Users\\Acer\\Desktop\\Hana\\suggest\\suggest\\x64\\Debug\\sample.xml";
    int userId = 1;

    // Generate the graph from the input XML file
    Graph* g = generateGraph(inputFile);

    suggest(g->getMap(), userId);

    return 0;
}
*/