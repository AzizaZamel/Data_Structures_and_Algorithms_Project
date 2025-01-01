#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <memory>
#include <set>
#include <map>
#include "user.hpp"
#include "xml_parser.hpp"
#include "xml_node.hpp"

// Parse comma-separated user IDs
std::vector<int> parse_ids(const std::string &ids_str);

// Display mutual followers in a formatted way
void display_mutual_followers(const std::vector<int> &user_ids,
                              const std::set<int> &mutual_followers,
                              const std::map<int, User> &user_map);

// Logger for error and status messages
void log_error(const std::string &message);
void log_info(const std::string &message);

// New helper function declarations
std::shared_ptr<xml_parser_lib::xml_node> find_child_by_name(
    const std::shared_ptr<xml_parser_lib::xml_node> &node,
    const std::string &child_name);

std::string get_child_text(
    const std::shared_ptr<xml_parser_lib::xml_node> &node,
    const std::string &child_name);

#endif // UTILS_HPP
