#ifndef XML_NODE_H
#define XML_NODE_H

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <iostream>

namespace xml_parser_lib
{

    class xml_node
    {
    public:
        xml_node(const std::string &name = "");
        ~xml_node() = default;

        // Setters
        void set_name(const std::string &name);
        void set_text(const std::string &text);
        void add_attribute(const std::string &key, const std::string &value);
        void add_child(std::shared_ptr<xml_node> child);

        // Getters
        const std::string &get_name() const;
        const std::string &get_text() const;
        const std::map<std::string, std::string> &get_attributes() const;
        const std::vector<std::shared_ptr<xml_node>> &get_children() const;

        // Utility
        void print(int indent = 0) const;

    private:
        std::string name_;
        std::string text_;
        std::map<std::string, std::string> attributes_;
        std::vector<std::shared_ptr<xml_node>> children_;
    };

} // namespace xml_parser_lib

#endif // XML_NODE_H

