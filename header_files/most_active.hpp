/********************  Author:Toqa Gamal Mohamed   *****************/
/********************  Project:DSA_project_part_2  *****************/
/********************  File:most_active.hpp        *****************/
#ifndef MOST_ACTIVE_USER_HPP // MOST_ACTIVE_USER_HPP
#define MOST_ACTIVE_USER_HPP

#include <unordered_map>
#include <string>
#include <list>
#include "graph.hpp"  

using namespace std;
// Function to find the most active user based on in-degree and out-degree
string most_active_user(Graph* g);

#endif 