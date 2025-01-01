#include "xml_parser.hpp"
#include <fstream>
#include <cctype>
#include <stdexcept>

namespace xml_parser_lib
{

    std::shared_ptr<xml_node> xml_parser::parse_from_string(const std::string &xml_content)
    {
        size_t pos = 0;
        skip_whitespace(xml_content, pos);

        if (pos >= xml_content.size() || xml_content[pos] != '<')
        {
            throw std::runtime_error("Invalid XML: Root element must start with '<'");
        }

        auto root = std::make_shared<xml_node>();
        parse_element(root, xml_content, pos);
        return root;
    }

    std::shared_ptr<xml_node> xml_parser::parse_from_file(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        file.close();

        return parse_from_string(content);
    }

    void xml_parser::skip_whitespace(const std::string &content, size_t &pos)
    {
        while (true)
        {
            // 1) Skip standard whitespace
            while (pos < content.size() && std::isspace(static_cast<unsigned char>(content[pos])))
            {
                pos++;
            }

            // 2) If we are at a comment start: <!--
            if (pos + 3 < content.size() && content[pos] == '<' &&
                content[pos + 1] == '!' && content[pos + 2] == '-' &&
                content[pos + 3] == '-')
            {
                pos += 4; // skip `<!--`
                // Now skip until we find `-->`
                bool found_end = false;
                while (pos + 2 < content.size())
                {
                    if (content[pos] == '-' && content[pos + 1] == '-' && content[pos + 2] == '>')
                    {
                        pos += 3; // skip `-->`
                        found_end = true;
                        break;
                    }
                    pos++;
                }
                if (!found_end)
                {
                    throw std::runtime_error("Unterminated comment starting at position " + std::to_string(pos));
                }
                // After skipping comment, continue the loop to skip more spaces/comments if present
            }
            else
            {
                // No more comments or whitespace to skip; break out
                break;
            }
        }
    }

    bool xml_parser::starts_with(const std::string &content, size_t pos, const std::string &prefix)
    {
        return content.compare(pos, prefix.length(), prefix) == 0;
    }

    std::string xml_parser::parse_tag_name(const std::string &content, size_t &pos)
    {
        size_t start = pos;
        while (pos < content.size() && (std::isalnum(static_cast<unsigned char>(content[pos])) || content[pos] == '_' || content[pos] == '-'))
        {
            pos++;
        }
        if (start == pos)
        {
            throw std::runtime_error("Expected tag name at position " + std::to_string(pos));
        }
        return content.substr(start, pos - start);
    }

    std::map<std::string, std::string> xml_parser::parse_attributes(const std::string &content, size_t &pos)
    {
        std::map<std::string, std::string> attributes;
        while (true)
        {
            skip_whitespace(content, pos);
            if (pos >= content.size())
            {
                throw std::runtime_error("Unexpected end of XML while parsing attributes.");
            }
            if (content[pos] == '>' || starts_with(content, pos, "/>"))
            {
                break;
            }

            // Parse attribute name
            std::string attr_name = parse_tag_name(content, pos);
            skip_whitespace(content, pos);

            if (pos >= content.size() || content[pos] != '=')
            {
                throw std::runtime_error("Expected '=' after attribute name at position " + std::to_string(pos));
            }
            pos++; // Skip '='
            skip_whitespace(content, pos);

            // Parse attribute value
            if (pos >= content.size() || (content[pos] != '"' && content[pos] != '\''))
            {
                throw std::runtime_error("Expected '\"' or '\\'' at position " + std::to_string(pos));
            }
            char quote = content[pos];
            pos++; // Skip quote

            size_t start = pos;
            while (pos < content.size() && content[pos] != quote)
            {
                pos++;
            }
            if (pos >= content.size())
            {
                throw std::runtime_error("Unterminated attribute value starting at position " + std::to_string(start));
            }
            std::string attr_value = content.substr(start, pos - start);
            pos++; // Skip closing quote

            attributes[attr_name] = attr_value;
        }
        return attributes;
    }

    std::string xml_parser::parse_text(const std::string &content, size_t &pos)
    {
        size_t start = pos;
        while (pos < content.size() && content[pos] != '<')
        {
            pos++;
        }
        return content.substr(start, pos - start);
    }

    void xml_parser::parse_element(std::shared_ptr<xml_node> current_node, const std::string &content, size_t &pos)
    {
        if (content[pos] != '<')
        {
            throw std::runtime_error("Expected '<' at position " + std::to_string(pos));
        }
        pos++; // Skip '<'

        // Check for closing tag
        if (pos < content.size() && content[pos] == '/')
        {
            // End of current element
            pos++; // Skip '/'
            std::string closing_tag = parse_tag_name(content, pos);
            skip_whitespace(content, pos);
            if (pos >= content.size() || content[pos] != '>')
            {
                throw std::runtime_error("Expected '>' at position " + std::to_string(pos));
            }
            pos++; // Skip '>'
            return;
        }

        // Parse tag name
        std::string tag_name = parse_tag_name(content, pos);
        current_node->set_name(tag_name);

        // Parse attributes
        std::map<std::string, std::string> attributes = parse_attributes(content, pos);
        for (const auto &[key, value] : attributes)
        {
            current_node->add_attribute(key, value);
        }

        skip_whitespace(content, pos);

        if (pos >= content.size())
        {
            throw std::runtime_error("Unexpected end of XML after attributes.");
        }

        if (content[pos] == '>')
        {
            pos++; // Skip '>'
        }
        else if (starts_with(content, pos, "/>"))
        {
            pos += 2; // Skip '/>'
            return;
        }
        else
        {
            throw std::runtime_error("Unexpected character at position " + std::to_string(pos));
        }

        // Parse children or text
        while (pos < content.size())
        {
            skip_whitespace(content, pos);
            if (starts_with(content, pos, "</"))
            {
                // Closing tag
                pos += 2; // Skip '</'
                std::string closing_tag = parse_tag_name(content, pos);
                skip_whitespace(content, pos);
                if (pos >= content.size() || content[pos] != '>')
                {
                    throw std::runtime_error("Expected '>' at position " + std::to_string(pos));
                }
                pos++; // Skip '>'
                if (closing_tag != tag_name)
                {
                    throw std::runtime_error("Mismatched closing tag: expected </" + tag_name + ">, found </" + closing_tag + ">");
                }
                return;
            }
            else if (content[pos] == '<')
            {
                // Child element
                auto child = std::make_shared<xml_node>();
                parse_element(child, content, pos);
                current_node->add_child(child);
            }
            else
            {
                // Text content
                std::string text = parse_text(content, pos);
                // Trim leading and trailing whitespace
                size_t first = text.find_first_not_of(" \t\n\r");
                size_t last = text.find_last_not_of(" \t\n\r");
                if (first != std::string::npos && last != std::string::npos)
                {
                    text = text.substr(first, last - first + 1);
                    current_node->set_text(text);
                }
            }
        }
    }

} // namespace xml_parser_lib
