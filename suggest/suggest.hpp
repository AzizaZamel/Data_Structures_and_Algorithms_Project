#ifndef SUGGEST_H
#define SUGGEST_H

#include <iostream>
#include <unordered_map>
#include <list>
#include <set>
#include <unordered_set>
#include "graph.hpp"
using namespace std;

// Function to determine who a given userId follows
vector<int> printWhoUserFollows(const unordered_map<int, list<int>>& m, int userId);

//Create suggestion from generated graph
unordered_set<int> suggest(const unordered_map<int, list<int>>& m, int userId);

//generates graph and runs suggest function
unordered_set<int> print_suggested(const string& filename, int userId);

//prints results of print_suggested function
void printS(unordered_set<int> result, int userId);

//turns result content to string and prints it
unordered_set<string> printS_string(const unordered_set<int>& result, int userId);

#endif