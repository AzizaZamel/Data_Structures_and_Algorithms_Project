#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <set>
#include "xml_node.hpp"
#include "user.hpp"

// Existing utility function declarations
std::vector<int> parse_ids(const std::string &ids_str);
void log_error(const std::string &message);
void log_info(const std::string &message);
void display_mutual_followers(const std::vector<int> &user_ids,
                              const std::set<int> &mutual_followers,
                              const std::map<int, User> &user_map);

// New helper function declarations
std::shared_ptr<xml_parser_lib::xml_node> find_child_by_name(
    const std::shared_ptr<xml_parser_lib::xml_node> &node,
    const std::string &child_name);

std::string get_child_text(
    const std::shared_ptr<xml_parser_lib::xml_node> &node,
    const std::string &child_name);

// New function declaration
std::string get_mutual_followers_string(const std::vector<int> &user_ids,
                                        const std::string &filename);

#endif // UTILS_HPP
