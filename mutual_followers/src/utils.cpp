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
