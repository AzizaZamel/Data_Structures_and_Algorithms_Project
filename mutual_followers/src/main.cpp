#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <stdexcept>
#include "xml_parser.hpp"
#include "xml_node.hpp"

// A struct representing each user in the network
struct User
{
    int id;
    std::string name;
    std::vector<int> followers;
};

int main(int argc, char *argv[])
{
    // Expected usage:
    //   xml_editor mutual -i <input_file.xml> -ids 1,2,3
    // We want to parse the command line in that flexible format.

    // We need "mutual", "-i", then a filename, then "-ids", then a comma list of IDs.
    if (argc < 6)
    {
        std::cerr << "Usage: " << argv[0]
                  << " mutual -i <network_xml> -ids <comma_separated_ids>\n";
        return 1;
    }

    bool foundMutual = false;
    std::string filename;
    std::string ids_str;

    // We'll parse the arguments in a loop.
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "mutual")
        {
            foundMutual = true;
        }
        else if (arg == "-i")
        {
            // next argument is the filename
            if (i + 1 < argc)
            {
                filename = argv[++i];
            }
            else
            {
                std::cerr << "Error: Missing file after -i.\n";
                return 1;
            }
        }
        else if (arg == "-ids")
        {
            // next argument is the comma list
            if (i + 1 < argc)
            {
                ids_str = argv[++i];
            }
            else
            {
                std::cerr << "Error: Missing IDs after -ids.\n";
                return 1;
            }
        }
    }

    // Check that we found "mutual", have a filename, and have IDs
    if (!foundMutual || filename.empty() || ids_str.empty())
    {
        std::cerr << "Error: Command must specify 'mutual', '-i file.xml', and '-ids <list>'.\n"
                  << "Usage: " << argv[0] << " mutual -i <network_xml> -ids 1,2,3\n";
        return 1;
    }

    // Parse the comma-separated IDs
    std::vector<int> user_ids;
    {
        size_t start = 0;
        while (true)
        {
            size_t comma_pos = ids_str.find(',', start);
            if (comma_pos == std::string::npos)
            {
                // last
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
        std::cerr << "Error: No valid user IDs after -ids.\n";
        return 1;
    }

    // Use xml_parser_lib to parse the network XML
    xml_parser_lib::xml_parser parser;
    std::shared_ptr<xml_parser_lib::xml_node> root_node;

    try
    {
        root_node = parser.parse_from_file(filename);
        if (!root_node)
        {
            std::cerr << "Error: Failed to parse XML from file: " << filename << "\n";
            return 1;
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error parsing XML: " << ex.what() << "\n";
        return 1;
    }

    // Check the root is <network>
    if (root_node->get_name() != "network")
    {
        std::cerr << "Error: Root element is not <network>. Found <"
                  << root_node->get_name() << "> instead.\n";
        return 1;
    }

    // Build a map<int,User> from the XML
    std::map<int, User> user_map;

    // Each child under <network> should be <user>
    for (auto &user_node : root_node->get_children())
    {
        if (user_node->get_name() != "user")
        {
            continue;
        }
        auto attrs = user_node->get_attributes();
        if (attrs.find("id") == attrs.end() || attrs.find("name") == attrs.end())
        {
            std::cerr << "Warning: <user> missing 'id' or 'name' attribute.\n";
            continue;
        }

        int uid = std::stoi(attrs.at("id"));
        std::string uname = attrs.at("name");

        User u;
        u.id = uid;
        u.name = uname;

        // Check <followers>
        for (auto &child : user_node->get_children())
        {
            if (child->get_name() == "followers")
            {
                // multiple <follower id="X"/>
                for (auto &ftag : child->get_children())
                {
                    if (ftag->get_name() == "follower")
                    {
                        auto fattrs = ftag->get_attributes();
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

    // For each requested user ID, gather the set of followers. We'll then intersect them.
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

    // Intersect all sets
    std::set<int> intersection_result = sets_of_followers[0];
    for (size_t i = 1; i < sets_of_followers.size(); ++i)
    {
        std::set<int> temp;
        std::set_intersection(intersection_result.begin(), intersection_result.end(),
                              sets_of_followers[i].begin(), sets_of_followers[i].end(),
                              std::inserter(temp, temp.begin()));
        intersection_result = temp;
    }

    // Print result
    if (!intersection_result.empty())
    {
        std::cout << "Mutual followers of users ";
        for (size_t i = 0; i < user_ids.size(); i++)
        {
            std::cout << user_ids[i];
            if (i + 1 < user_ids.size())
                std::cout << ",";
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
                std::cout << ",";
        }
        std::cout << ".\n";
    }

    return 0;
}
