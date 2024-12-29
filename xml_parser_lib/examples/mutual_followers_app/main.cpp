#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stdexcept>
#include <algorithm>
#include "xml_parser.hpp"
#include "xml_node.hpp"

// Structure to store each user's data
struct User
{
    int id = -1;
    std::string name;
    std::vector<int> followers;
};

int main(int argc, char *argv[])
{
    // Expect exactly 6 args if we want the format:
    // argv[0] = "xml_editor" (the program name, for example)
    // argv[1] = "mutual"
    // argv[2] = "-i"
    // argv[3] = "input_file.xml"
    // argv[4] = "-ids"
    // argv[5] = "1,2,3"

    if (argc != 6)
    {
        std::cerr << "Usage: " << argv[0]
                  << " mutual -i <path_to_network_xml> -ids <comma_separated_ids>\n";
        return 1;
    }

    // 1) Check arguments
    if (std::string(argv[1]) != "mutual")
    {
        std::cerr << "Error: First argument must be 'mutual'.\n"
                  << "Usage: " << argv[0]
                  << " mutual -i <path_to_network_xml> -ids <comma_separated_ids>\n";
        return 1;
    }

    if (std::string(argv[2]) != "-i")
    {
        std::cerr << "Error: Second argument must be '-i'.\n"
                  << "Usage: " << argv[0]
                  << " mutual -i <path_to_network_xml> -ids <comma_separated_ids>\n";
        return 1;
    }

    std::string filename = argv[3];

    if (std::string(argv[4]) != "-ids")
    {
        std::cerr << "Error: Fourth argument must be '-ids'.\n"
                  << "Usage: " << argv[0]
                  << " mutual -i <path_to_network_xml> -ids <comma_separated_ids>\n";
        return 1;
    }

    // argv[5] => "1,2,3"
    std::string ids_str = argv[5];

    // 2) Parse user IDs from the comma-separated string
    std::vector<int> user_ids;
    {
        size_t start = 0;
        while (true)
        {
            size_t comma_pos = ids_str.find(',', start);
            if (comma_pos == std::string::npos)
            {
                // last ID
                std::string id_str = ids_str.substr(start);
                if (!id_str.empty())
                {
                    user_ids.push_back(std::stoi(id_str));
                }
                break;
            }
            else
            {
                std::string id_str = ids_str.substr(start, comma_pos - start);
                if (!id_str.empty())
                {
                    user_ids.push_back(std::stoi(id_str));
                }
                start = comma_pos + 1;
            }
        }
    }

    if (user_ids.empty())
    {
        std::cerr << "Error: No valid user IDs found in '-ids' argument.\n";
        return 1;
    }

    // 3) Parse the XML with xml_parser_lib
    xml_parser_lib::xml_parser parser;
    std::shared_ptr<xml_parser_lib::xml_node> root_node;

    try
    {
        root_node = parser.parse_from_file(filename);
        if (!root_node)
        {
            std::cerr << "Failed to parse XML.\n";
            return 1;
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error parsing XML: " << ex.what() << "\n";
        return 1;
    }

    if (root_node->get_name() != "network")
    {
        std::cerr << "Error: Root element is not <network>. Found <"
                  << root_node->get_name() << "> instead.\n";
        return 1;
    }

    // 4) Build a map of user_id -> User
    std::map<int, User> user_map;

    for (auto &user_node : root_node->get_children())
    {
        if (user_node->get_name() != "user")
            continue;

        auto attrs = user_node->get_attributes();
        if (attrs.find("id") == attrs.end() || attrs.find("name") == attrs.end())
        {
            std::cerr << "Skipping <user> missing 'id' or 'name'.\n";
            continue;
        }
        int uid = std::stoi(attrs.at("id"));
        std::string uname = attrs.at("name");

        User u;
        u.id = uid;
        u.name = uname;

        // parse <followers> block
        for (auto &child : user_node->get_children())
        {
            if (child->get_name() == "followers")
            {
                for (auto &follower_tag : child->get_children())
                {
                    if (follower_tag->get_name() == "follower")
                    {
                        auto fattrs = follower_tag->get_attributes();
                        if (fattrs.find("id") != fattrs.end())
                        {
                            int fid = std::stoi(fattrs.at("id"));
                            u.followers.push_back(fid);
                        }
                    }
                }
            }
        }

        user_map[u.id] = u;
    }

    // 5) Identify which users follow all of the specified user IDs
    // We'll gather sets of each user's followers, then find intersection
    std::vector<std::set<int>> sets_of_followers;
    sets_of_followers.reserve(user_ids.size());

    for (int uid : user_ids)
    {
        if (user_map.find(uid) == user_map.end())
        {
            std::cerr << "User ID " << uid << " not found in the XML.\n";
            return 1;
        }
        std::set<int> fset(user_map[uid].followers.begin(), user_map[uid].followers.end());
        sets_of_followers.push_back(fset);
    }

    // Intersection across all sets
    std::set<int> intersection_result = sets_of_followers[0];
    for (size_t i = 1; i < sets_of_followers.size(); ++i)
    {
        std::set<int> temp;
        std::set_intersection(intersection_result.begin(), intersection_result.end(),
                              sets_of_followers[i].begin(), sets_of_followers[i].end(),
                              std::inserter(temp, temp.begin()));
        intersection_result = temp;
    }

    // 6) Print the mutual followers
    if (!intersection_result.empty())
    {
        std::cout << "Mutual followers of users ";
        for (size_t i = 0; i < user_ids.size(); i++)
        {
            std::cout << user_ids[i];
            if (i + 1 < user_ids.size())
                std::cout << ", ";
        }
        std::cout << ":\n";

        for (int fid : intersection_result)
        {
            if (user_map.find(fid) != user_map.end())
            {
                std::cout << "  " << user_map[fid].name << " (ID=" << fid << ")\n";
            }
            else
            {
                std::cout << "  Unknown user with ID=" << fid << "\n";
            }
        }
    }
    else
    {
        std::cout << "No mutual followers found for users ";
        for (size_t i = 0; i < user_ids.size(); i++)
        {
            std::cout << user_ids[i];
            if (i + 1 < user_ids.size())
                std::cout << ", ";
        }
        std::cout << ".\n";
    }

    return 0;
}
