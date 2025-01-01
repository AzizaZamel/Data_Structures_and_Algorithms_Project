#include "xml_util.hpp"

namespace xml_parser_lib::util
{

    void find_nodes_by_name(const std::shared_ptr<xml_node> &root,
                            const std::string &name,
                            std::vector<std::shared_ptr<xml_node>> &result)
    {
        if (!root)
            return;

        // If this node's name matches, add it
        if (root->get_name() == name)
        {
            result.push_back(root);
        }

        // Recursively traverse children
        for (const auto &child : root->get_children())
        {
            find_nodes_by_name(child, name, result);
        }
    }

} // namespace xml_parser_lib::util
