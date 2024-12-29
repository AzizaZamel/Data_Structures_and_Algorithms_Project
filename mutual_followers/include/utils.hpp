#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <set>
#include <map>
#include "user.hpp"

// Parse comma-separated user IDs
std::vector<int> parse_ids(const std::string &ids_str);

// Display mutual followers in a formatted way
void display_mutual_followers(const std::vector<int> &user_ids,
                              const std::set<int> &mutual_followers,
                              const std::map<int, User> &user_map);

// Logger for error and status messages
void log_error(const std::string &message);
void log_info(const std::string &message);

#endif // UTILS_HPP
