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

void suggest(const unordered_map<int, list<int>>& m, int userId);

#endif