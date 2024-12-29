#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "xml_parser.hpp"
#include "xml_util.hpp"
#include "xml_node.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: util_app <path_to_xml_file>\n";
        return 1;
    }

    std::string filename = argv[1];
    xml_parser_lib::xml_parser parser;

    try
    {
        // Parse the file
        std::shared_ptr<xml_parser_lib::xml_node> root = parser.parse_from_file(filename);
        if (!root)
        {
            std::cerr << "Failed to parse XML.\n";
            return 1;
        }

        std::cout << "Parsed root node: " << root->get_name() << "\n";

        // Decide what to search for based on root name
        std::string target_name;
        if (root->get_name() == "library")
        {
            target_name = "book";
        }
        else if (root->get_name() == "network")
        {
            target_name = "user";
        }
        else
        {
            std::cout << "Unknown root. Searching for <greeting> as a demo.\n";
            target_name = "greeting";
        }

        // Perform the search
        std::vector<std::shared_ptr<xml_parser_lib::xml_node>> found;
        xml_parser_lib::util::find_nodes_by_name(root, target_name, found);

        std::cout << "Found " << found.size() << " <" << target_name << "> node(s).\n\n";

        // Print some details from each found node
        for (auto &node : found)
        {
            std::cout << "<" << node->get_name();
            // Print attributes
            for (const auto &[k, v] : node->get_attributes())
            {
                std::cout << " " << k << "=\"" << v << "\"";
            }
            std::cout << ">";
            if (!node->get_text().empty())
            {
                std::cout << node->get_text();
            }
            std::cout << "</" << node->get_name() << ">\n";
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
