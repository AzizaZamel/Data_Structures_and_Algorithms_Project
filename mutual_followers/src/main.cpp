#include <iostream>
#include <memory>
#include <map>
#include <set>
#include "xml_parser.hpp"
#include "xml_node.hpp"
#include "mutual_followers.hpp"
#include "user.hpp"
#include "utils.hpp"

int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        std::cerr << "Usage: " << argv[0] << " mutual -i <users_xml> -ids 1,2,3" << std::endl;
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

    if (root_node->get_name() != "users")
    {
        log_error("Root element is not <users>.");
        return 1;
    }

    std::map<int, User> user_map;
    for (const auto &user_node : root_node->get_children())
    {
        if (user_node->get_name() != "user")
            continue;

        // Extract <id>
        std::string id_text = get_child_text(user_node, "id");
        if (id_text.empty())
        {
            log_error("Missing <id> in <user>.");
            continue;
        }

        int uid;
        try
        {
            uid = std::stoi(id_text);
        }
        catch (const std::exception &)
        {
            log_error("Invalid <id> value in <user>.");
            continue;
        }

        // Extract <name>
        std::string uname = get_child_text(user_node, "name");
        if (uname.empty())
        {
            log_error("Missing <name> in <user>.");
            continue;
        }

        User u{uid, uname, {}};

        // Extract followers
        auto followers_node = find_child_by_name(user_node, "followers");
        if (followers_node)
        {
            for (const auto &follower_node : followers_node->get_children())
            {
                if (follower_node->get_name() == "follower")
                {
                    std::string fid_text = get_child_text(follower_node, "id");
                    if (!fid_text.empty())
                    {
                        try
                        {
                            int fid = std::stoi(fid_text);
                            u.followers.push_back(fid);
                        }
                        catch (const std::exception &)
                        {
                            log_error("Invalid follower <id> in <follower>.");
                        }
                    }
                    else
                    {
                        log_error("Missing <id> in <follower>.");
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
