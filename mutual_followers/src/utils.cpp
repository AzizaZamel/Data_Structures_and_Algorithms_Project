#include "mutual_followers.hpp"
#include "xml_parser.hpp"
#include "xml_node.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>

// Existing utility function implementations
std::vector<int> parse_ids(const std::string &ids_str)
{
    std::vector<int> user_ids;
    std::stringstream ss(ids_str);
    std::string item;
    while (std::getline(ss, item, ','))
    {
        if (!item.empty())
        {
            try
            {
                user_ids.push_back(std::stoi(item));
            }
            catch (const std::invalid_argument &)
            {
                log_error("Invalid ID encountered: " + item);
            }
            catch (const std::out_of_range &)
            {
                log_error("ID out of range: " + item);
            }
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

// New helper function implementations

std::shared_ptr<xml_parser_lib::xml_node> find_child_by_name(
    const std::shared_ptr<xml_parser_lib::xml_node> &node,
    const std::string &child_name)
{
    for (const auto &child : node->get_children())
    {
        if (child->get_name() == child_name)
        {
            return child;
        }
    }
    return nullptr;
}

std::string get_child_text(
    const std::shared_ptr<xml_parser_lib::xml_node> &node,
    const std::string &child_name)
{
    auto child = find_child_by_name(node, child_name);
    if (child)
    {
        return child->get_text();
    }
    return "";
}

// New function implementation
std::string get_mutual_followers_string(const std::vector<int> &user_ids,
                                        const std::string &filename)
{
    xml_parser_lib::xml_parser parser;
    std::shared_ptr<xml_parser_lib::xml_node> root_node;
    std::stringstream ss;

    try
    {
        root_node = parser.parse_from_file(filename);
    }
    catch (const std::exception &ex)
    {
        ss << "[ERROR]: Failed to parse XML: " << ex.what() << "\n";
        return ss.str();
    }

    if (root_node->get_name() != "users")
    {
        ss << "[ERROR]: Root element is not <users>.\n";
        return ss.str();
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
            ss << "[ERROR]: Missing <id> in <user>.\n";
            continue;
        }

        int uid;
        try
        {
            uid = std::stoi(id_text);
        }
        catch (const std::exception &)
        {
            ss << "[ERROR]: Invalid <id> value in <user>.\n";
            continue;
        }

        // Extract <name>
        std::string uname = get_child_text(user_node, "name");
        if (uname.empty())
        {
            ss << "[ERROR]: Missing <name> in <user>.\n";
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
                            ss << "[ERROR]: Invalid follower <id> in <follower>.\n";
                        }
                    }
                    else
                    {
                        ss << "[ERROR]: Missing <id> in <follower>.\n";
                    }
                }
            }
        }

        user_map[u.id] = u;
    }

    // Find mutual followers
    std::set<int> mutual_followers;
    try
    {
        mutual_followers = find_mutual_followers(user_map, user_ids);
    }
    catch (const std::exception &ex)
    {
        ss << "[ERROR]: Error finding mutual followers: " << ex.what() << "\n";
        return ss.str();
    }

    // Format the output
    if (mutual_followers.empty())
    {
        ss << "Mutual followers of users: ";
        for (size_t i = 0; i < user_ids.size(); ++i)
        {
            ss << user_ids[i] << (i + 1 < user_ids.size() ? ", " : "\n");
        }
        ss << "[INFO]: No mutual followers found.\n";
        return ss.str();
    }

    ss << "Mutual followers of users: ";
    for (size_t i = 0; i < user_ids.size(); ++i)
    {
        ss << user_ids[i] << (i + 1 < user_ids.size() ? ", " : "\n");
    }

    for (int fid : mutual_followers)
    {
        auto it = user_map.find(fid);
        if (it != user_map.end())
        {
            ss << "  " << it->second.name << " (ID=" << fid << ")\n";
        }
    }

    return ss.str();
}
