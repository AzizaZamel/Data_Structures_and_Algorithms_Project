#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <string>
#include <memory>
#include "xml_node.hpp"

namespace xml_parser_lib
{

    class xml_parser
    {
    public:
        xml_parser() = default;
        ~xml_parser() = default;

        // Parses XML content from a string and returns the root node
        std::shared_ptr<xml_node> parse_from_string(const std::string &xml_content);

        // Parses XML content from a file and returns the root node
        std::shared_ptr<xml_node> parse_from_file(const std::string &filename);

    private:
        // Helper methods
        void skip_whitespace(const std::string &content, size_t &pos);
        bool starts_with(const std::string &content, size_t pos, const std::string &prefix);
        std::string parse_tag_name(const std::string &content, size_t &pos);
        std::map<std::string, std::string> parse_attributes(const std::string &content, size_t &pos);
        std::string parse_text(const std::string &content, size_t &pos);

        // Recursive parsing function
        void parse_element(std::shared_ptr<xml_node> current_node, const std::string &content, size_t &pos);
    };

} // namespace xml_parser_lib

#endif // XML_PARSER_H

