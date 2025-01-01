#include "xml_node.hpp"

namespace xml_parser_lib
{

    xml_node::xml_node(const std::string &name) : name_(name), text_("") {}

    void xml_node::set_name(const std::string &name)
    {
        name_ = name;
    }

    void xml_node::set_text(const std::string &text)
    {
        text_ = text;
    }

    void xml_node::add_attribute(const std::string &key, const std::string &value)
    {
        attributes_[key] = value;
    }

    void xml_node::add_child(std::shared_ptr<xml_node> child)
    {
        children_.push_back(child);
    }

    const std::string &xml_node::get_name() const
    {
        return name_;
    }

    const std::string &xml_node::get_text() const
    {
        return text_;
    }

    const std::map<std::string, std::string> &xml_node::get_attributes() const
    {
        return attributes_;
    }

    const std::vector<std::shared_ptr<xml_node>> &xml_node::get_children() const
    {
        return children_;
    }

    void xml_node::print(int indent) const
    {
        std::string indentation(indent, ' ');
        std::cout << indentation << "<" << name_;

        // Print attributes
        for (auto it = attributes_.begin(); it != attributes_.end(); ++it)
        {
            std::cout << " " << it->first << "=\"" << it->second << "\"";
        }

        if (children_.empty() && text_.empty())
        {
            std::cout << " />\n";
            return;
        }

        std::cout << ">";

        // Print text if available
        if (!text_.empty())
        {
            std::cout << text_;
        }

        std::cout << "\n";

        // Print children
        for (const auto &child : children_)
        {
            child->print(indent + 2);
        }

        std::cout << indentation << "</" << name_ << ">\n";
    }

} // namespace xml_parser_lib
