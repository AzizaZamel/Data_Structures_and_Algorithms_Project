#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "xml_parser.hpp"
#include "xml_node.hpp"

// struct to store each user's info
struct User
{
    int id = -1;
    std::string name;
    std::vector<int> followers;
};

int main(int argc, char *argv[])
{
    // We expect exactly 4 arguments:
    // argv[0] = "xml_editor" (the program name, for example)
    // argv[1] = "most_influencer"
    // argv[2] = "-i"
    // argv[3] = "input_file.xml"
    //
    // So total = 4, or adapt as you wish.

    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " most_influencer -i <path_to_network_xml>\n";
        return 1;
    }

    // Check argv[1] == "most_influencer", argv[2] == "-i"
    if (std::string(argv[1]) != "most_influencer" || std::string(argv[2]) != "-i")
    {
        std::cerr << "Invalid command.\n"
                  << "Usage: " << argv[0] << " most_influencer -i <path_to_network_xml>\n";
        return 1;
    }

    // Now we assume argv[3] is the filename
    std::string filename = argv[3];

    {
        // 1. Parse the XML
        xml_parser_lib::xml_parser parser;
        std::shared_ptr<xml_parser_lib::xml_node> root_node;

        try
        {
            root_node = parser.parse_from_file(filename);

            if (!root_node)
            {
                std::cerr << "Error: Failed to parse XML.\n";
                return 1;
            }
        }
        catch (const std::exception &ex)
        {
            std::cerr << "Error parsing XML: " << ex.what() << "\n";
            return 1;
        }

        // 2. Ensure the root element is "network"
        if (root_node->get_name() != "network")
        {
            std::cerr << "Error: Root element is not <network>. Found <"
                      << root_node->get_name() << "> instead.\n";
            return 1;
        }

        // 3. Build a map of user_id -> User data
        std::map<int, User> user_map;

        // Each child under <network> is a <user> node
        for (const auto &user_node : root_node->get_children())
        {
            if (user_node->get_name() != "user")
            {
                continue; // skip anything not <user>
            }

            // Extract user attributes: id, name
            auto attrs = user_node->get_attributes();
            if (attrs.find("id") == attrs.end() || attrs.find("name") == attrs.end())
            {
                std::cerr << "Warning: <user> missing 'id' or 'name' attribute.\n";
                continue;
            }

            int user_id = std::stoi(attrs.at("id"));
            std::string user_name = attrs.at("name");

            User user;
            user.id = user_id;
            user.name = user_name;

            // 4. Extract <followers> block
            //    Each <follower id="X" /> means user X follows *this* user
            for (const auto &child : user_node->get_children())
            {
                if (child->get_name() == "followers")
                {
                    // Inside <followers>, we have multiple <follower id="..."/>
                    for (const auto &follower_tag : child->get_children())
                    {
                        if (follower_tag->get_name() == "follower")
                        {
                            auto follower_attrs = follower_tag->get_attributes();
                            if (follower_attrs.find("id") != follower_attrs.end())
                            {
                                int follower_id = std::stoi(follower_attrs.at("id"));
                                user.followers.push_back(follower_id);
                            }
                        }
                    }
                }
            }

            // Store in map
            user_map[user.id] = user;
        }

        // 5. Identify the "influencer" user = the one with the most followers
        int max_followers = -1;
        int influencer_id = -1;

        for (const auto &[id, user] : user_map)
        {
            int count = static_cast<int>(user.followers.size());
            if (count > max_followers)
            {
                max_followers = count;
                influencer_id = id;
            }
        }

        if (influencer_id == -1)
        {
            std::cerr << "No valid users found.\n";
            return 1;
        }

        const User &influencer = user_map[influencer_id];
        std::cout << "Most Influencer User:\n";
        std::cout << "  ID: " << influencer.id << "\n";
        std::cout << "  Name: " << influencer.name << "\n";
        std::cout << "  Followers Count: " << influencer.followers.size() << "\n";
    }

    return 0;
}
