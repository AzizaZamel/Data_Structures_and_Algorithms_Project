// File: include/xml_util.hpp

#ifndef XML_UTIL_HPP
#define XML_UTIL_HPP

#include <string>
#include <vector>
#include <memory>
#include "xml_node.hpp"

namespace xml_parser_lib::util
{

    // A utility function to find all nodes with a given name in the XML tree
    void find_nodes_by_name(const std::shared_ptr<xml_node> &root,
                            const std::string &name,
                            std::vector<std::shared_ptr<xml_node>> &result);

} // namespace xml_parser_lib::util

#endif // XML_UTIL_HPP
