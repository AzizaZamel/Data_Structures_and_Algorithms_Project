#ifndef MUTUAL_FOLLOWERS_HPP
#define MUTUAL_FOLLOWERS_HPP

#include <map>
#include <set>
#include <vector>
#include "user.hpp"

// Function to find mutual followers
std::set<int> find_mutual_followers(const std::map<int, User> &user_map,
                                    const std::vector<int> &user_ids);

#endif // MUTUAL_FOLLOWERS_HPP
