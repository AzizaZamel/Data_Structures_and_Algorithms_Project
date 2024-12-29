// utils.cpp
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <vector>

std::vector<int> parse_ids(const std::string &ids_str)
{
    std::vector<int> user_ids;
    std::stringstream ss(ids_str);
    std::string item;
    while (std::getline(ss, item, ','))
    {
        if (!item.empty())
        {
            user_ids.push_back(std::stoi(item));
        }
    }
    return user_ids;
}

void display_mutual_followers(const std::vector<int> &user_ids,
                              const std::set<int> &mutual_followers,
                              const std::map<int, User> &user_map)
{
    if (mutual_followers.empty())
    {
        log_info("No mutual followers found.");
        return;
    }

    std::cout << "Mutual followers of users: ";
    for (size_t i = 0; i < user_ids.size(); ++i)
    {
        std::cout << user_ids[i] << (i + 1 < user_ids.size() ? ", " : "\n");
    }

    for (int fid : mutual_followers)
    {
        auto it = user_map.find(fid);
        if (it != user_map.end())
        {
            std::cout << "  " << it->second.name << " (ID=" << fid << ")\n";
        }
    }
}

void log_error(const std::string &message)
{
    std::cerr << "[ERROR]: " << message << "\n";
}

void log_info(const std::string &message)
{
    std::cout << "[INFO]: " << message << "\n";
}
