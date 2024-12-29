#include <iostream>
#include <memory>
#include <map>
#include <set>
#include "mutual_followers.hpp"
#include "xml_parser.hpp"
#include "user.hpp"
#include "utils.hpp"

int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        std::cerr << "Usage: " << argv[0] << " mutual -i <network_xml> -ids 1,2,3" << std::endl;
        return 1;
    }

    std::string filename, ids_str;
    bool foundMutual = false;

    // Parse CLI arguments
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "mutual")
        {
            foundMutual = true;
        }
        else if (arg == "-i")
        {
            if (++i < argc)
                filename = argv[i];
        }
        else if (arg == "-ids")
        {
            if (++i < argc)
                ids_str = argv[i];
        }
    }

    if (!foundMutual || filename.empty() || ids_str.empty())
    {
        log_error("Command must specify 'mutual', '-i file.xml', and '-ids <list>'.");
        return 1;
    }

    auto user_ids = parse_ids(ids_str);
    if (user_ids.empty())
    {
        log_error("No valid user IDs provided.");
        return 1;
    }

    xml_parser_lib::xml_parser parser;
    std::shared_ptr<xml_parser_lib::xml_node> root_node;
    try
    {
        root_node = parser.parse_from_file(filename);
    }
    catch (const std::exception &ex)
    {
        log_error("Failed to parse XML: " + std::string(ex.what()));
        return 1;
    }

    if (root_node->get_name() != "network")
    {
        log_error("Root element is not <network>.");
        return 1;
    }

    std::map<int, User> user_map;
    for (auto &user_node : root_node->get_children())
    {
        if (user_node->get_name() != "user")
            continue;

        auto attrs = user_node->get_attributes();
        if (attrs.find("id") == attrs.end() || attrs.find("name") == attrs.end())
        {
            log_error("Missing 'id' or 'name' in <user>.");
            continue;
        }

        int uid = std::stoi(attrs.at("id"));
        std::string uname = attrs.at("name");
        User u{uid, uname, {}};

        for (auto &child : user_node->get_children())
        {
            if (child->get_name() == "followers")
            {
                for (auto &f : child->get_children())
                {
                    if (f->get_name() == "follower")
                    {
                        auto fattrs = f->get_attributes();
                        if (fattrs.find("id") != fattrs.end())
                        {
                            u.followers.push_back(std::stoi(fattrs.at("id")));
                        }
                    }
                }
            }
        }
        user_map[u.id] = u;
    }

    try
    {
        auto mutual_followers = find_mutual_followers(user_map, user_ids);
        display_mutual_followers(user_ids, mutual_followers, user_map);
    }
    catch (const std::exception &ex)
    {
        log_error("Error finding mutual followers: " + std::string(ex.what()));
    }

    return 0;
}
