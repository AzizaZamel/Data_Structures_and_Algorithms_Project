#include "mutual_followers.hpp"
#include <algorithm>
#include <stdexcept>

// Implementation of the mutual follower logic
std::set<int> find_mutual_followers(const std::map<int, User> &user_map,
                                    const std::vector<int> &user_ids)
{
    std::vector<std::set<int>> sets_of_followers;
    sets_of_followers.reserve(user_ids.size());

    // Collect followers of each user
    for (int uid : user_ids)
    {
        if (user_map.find(uid) == user_map.end())
        {
            throw std::runtime_error("User ID " + std::to_string(uid) + " not found.");
        }
        std::set<int> fset(user_map.at(uid).followers.begin(),
                           user_map.at(uid).followers.end());
        sets_of_followers.push_back(fset);
    }

    // Intersect all sets to find mutual followers
    std::set<int> intersection_result = sets_of_followers[0];
    for (size_t i = 1; i < sets_of_followers.size(); ++i)
    {
        std::set<int> temp;
        std::set_intersection(intersection_result.begin(), intersection_result.end(),
                              sets_of_followers[i].begin(), sets_of_followers[i].end(),
                              std::inserter(temp, temp.begin()));
        intersection_result = temp;
    }

    return intersection_result;
}
