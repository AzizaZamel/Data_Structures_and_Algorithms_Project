#include <iostream>
#include <memory>
#include <string>
#include "xml_node.hpp"
#include "xml_parser.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: util_app <path_to_xml_file>\n";
        return 1;
    }
    std::string filename = argv[1];

    // Create an instance of the XML parser
    xml_parser_lib::xml_parser parser;

    try
    {
        // Parse from file instead of string
        std::shared_ptr<xml_parser_lib::xml_node> root_node = parser.parse_from_file(filename);

        if (root_node)
        {
            std::cout << "XML Parsed Successfully.\n";
            std::cout << "Parsed XML Structure:\n";
            root_node->print();
        }
        else
        {
            std::cerr << "Failed to parse XML.\n";
            return 1;
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error parsing XML: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
