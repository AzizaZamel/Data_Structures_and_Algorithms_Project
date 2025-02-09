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

unordered_set<int> suggest(const unordered_map<int, list<int>>& m, int userId) {
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
    return result;
}

unordered_set<int> print_suggested(const string& filename, int userId) {
    Graph* g = generateGraph(filename);

    return suggest(g->getMap(), userId);

}

void printS(unordered_set<int> result, int userId) {
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

string printS_string(const unordered_set<int>& result, int userId) {
    string stringResult = "User "+to_string(userId)+" suggestions are: \n"; // To store the string type of the result

    if (result.empty()) {
        stringResult = "No suggestions available.\n";
        return stringResult;
    }
    else {
        for (int suggestion : result) {
            string suggestionStr = to_string(suggestion); // Convert int to string
            stringResult += suggestionStr + " ";           // Add to string
        }
    }
    return stringResult; // Return suggested users as string
}

/*
//usage example:
int main(int argc, char* argv[]) {
    unordered_set<int> result;
    string inputFile = "C:\\Users\\Acer\\Desktop\\Hana\\suggest\\suggest\\x64\\Debug\\sample.xml";
    int userId = 1;

    result = print_suggested(inputFile, userId);
    cout << printS_string(result, userId);

    return 0;
}
*/
