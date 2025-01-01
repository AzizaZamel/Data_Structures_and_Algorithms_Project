#include "xml_checker.hpp"
#include <iostream>
#include <stack>
#include <fstream>
#include <cctype>

// Struct to store tag information
struct TagInfo
{
    std::string name;
    int line;
    int column;
};

// Helper function to trim whitespace from both ends of a string
static std::string trim(const std::string &str)
{
    size_t start = 0;
    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start])))
        start++;
    size_t end = str.size();
    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1])))
        end--;
    return str.substr(start, end - start);
}

void XmlChecker::reportError(const std::string &message, int line, int column)
{
    std::cerr << "Error (Line " << line << ", Column " << column << "): "
              << message << std::endl;
}

//------------------------------------------------------------------------------
// checkConsistency Implementation
//------------------------------------------------------------------------------
int XmlChecker::checkConsistency(const std::string &xmlContent)
{
    // Stack to keep track of opening tags and their line/column numbers
    std::stack<TagInfo> tagStack;

    int errors = 0;
    int line = 1;
    int column = 0;
    bool inTag = false;     // Are we currently reading inside a <...>?
    bool inComment = false; // Are we inside a comment?
    std::string currentTag;
    TagInfo currentTagInfo;

    for (size_t i = 0; i < xmlContent.size(); ++i)
    {
        char c = xmlContent[i];

        // Track line and column
        if (c == '\n')
        {
            line++;
            column = 0;
        }
        else
        {
            column++;
        }

        // Handle comment start
        if (!inComment && !inTag && c == '<' && (i + 3 < xmlContent.size()) &&
            xmlContent[i + 1] == '!' && xmlContent[i + 2] == '-' && xmlContent[i + 3] == '-')
        {
            inComment = true;
            i += 3; // Skip the '!--'
            column += 3;
            continue;
        }

        // Handle comment end
        if (inComment)
        {
            if (c == '-' && (i + 2 < xmlContent.size()) &&
                xmlContent[i + 1] == '-' && xmlContent[i + 2] == '>')
            {
                inComment = false;
                i += 2; // Skip the '-->'
                column += 2;
            }
            continue; // Skip all content inside comments
        }

        // Check for start of tag
        if (c == '<' && !inTag)
        {
            inTag = true;
            currentTag.clear();
            // Record the position where the tag starts
            currentTagInfo.line = line;
            currentTagInfo.column = column;
            continue;
        }
        // Check for end of tag
        else if (c == '>' && inTag)
        {
            inTag = false;

            // Trim the tag content
            std::string trimmedTag = trim(currentTag);

            if (trimmedTag.empty())
            {
                // Empty tag, ignore
                continue;
            }

            // Determine if it's a closing tag
            bool isClosing = false;
            size_t pos = 0;
            if (trimmedTag[0] == '/')
            {
                isClosing = true;
                pos = 1; // Skip '/'
            }

            // Extract tag name
            std::string tagName;
            while (pos < trimmedTag.size() &&
                   !std::isspace(static_cast<unsigned char>(trimmedTag[pos])) &&
                   trimmedTag[pos] != '/')
            {
                tagName += trimmedTag[pos];
                pos++;
            }

            tagName = trim(tagName);

            // Determine if it's a self-closing tag
            bool isSelfClosing = false;
            if (!isClosing && !trimmedTag.empty() && trimmedTag.back() == '/')
            {
                isSelfClosing = true;
            }

            if (isSelfClosing)
            {
                // Self-closing tag; no need to push to stack
                continue;
            }

            if (isClosing)
            {
                if (tagStack.empty())
                {
                    // No tag in stack -> unexpected closing tag
                    reportError("Unexpected closing tag </" + tagName + ">", currentTagInfo.line, currentTagInfo.column);
                    errors++;
                }
                else
                {
                    // Check if the top of the stack matches
                    if (tagStack.top().name == tagName)
                    {
                        // Perfect match
                        tagStack.pop();
                    }
                    else
                    {
                        // Mismatched tag
                        reportError("Mismatched tag. Expected </" + tagStack.top().name + "> but found </" + tagName + ">",
                                    currentTagInfo.line, currentTagInfo.column);
                        errors++;
                        // Pop the top to prevent duplicate errors
                        tagStack.pop();
                    }
                }
            }
            else
            {
                // Opening tag -> push onto stack 
                TagInfo tagInfo;
                tagInfo.name = tagName;
                tagInfo.line = currentTagInfo.line;
                tagInfo.column = currentTagInfo.column;
                tagStack.push(tagInfo);
            }
        }
        else if (inTag)
        {
            // Build up the tag content (including '/', attributes, etc.)
            currentTag += c;
        }
    }

    // After the loop, any tags left in the stack are unclosed
    while (!tagStack.empty())
    {
        auto unclosed = tagStack.top();
        tagStack.pop();
        reportError("Unclosed tag <" + unclosed.name + ">", unclosed.line, unclosed.column);
        errors++;
    }

    // Print final result
    if (errors == 0)
    {
        std::cout << "valid" << std::endl;
    }
    else
    {
        std::cout << "invalid" << std::endl;
        std::cout << "Total Errors: " << errors << std::endl;
    }

    return errors;
}

//------------------------------------------------------------------------------
// fixXml Implementation
//------------------------------------------------------------------------------
void XmlChecker::fixXml(const std::string &xmlContent, const std::string &outputPath)
{
    // We'll parse similarly, but build a "fixed" XML string
    std::stack<std::string> tagStack;
    std::string fixedXml;

    int line = 1;
    int column = 0;
    bool inTag = false;
    bool inComment = false;
    std::string currentTag;
    std::string tagBuffer;

    for (size_t i = 0; i < xmlContent.size(); ++i)
    {
        char c = xmlContent[i];

        // Track line and column
        if (c == '\n')
        {
            line++;
            column = 0;
        }
        else
        {
            column++;
        }

        // Handle comment start
        if (!inComment && !inTag && c == '<' && (i + 3 < xmlContent.size()) &&
            xmlContent[i + 1] == '!' && xmlContent[i + 2] == '-' && xmlContent[i + 3] == '-')
        {
            inComment = true;
            fixedXml += c;
            i += 3; // Skip the '!--'
            fixedXml += "!--";
            column += 3;
            continue;
        }

        // Handle comment end
        if (inComment)
        {
            fixedXml += c;
            if (c == '-' && (i + 2 < xmlContent.size()) &&
                xmlContent[i + 1] == '-' && xmlContent[i + 2] == '>')
            {
                inComment = false;
                i += 2; // Skip the '-->'
                fixedXml += "->";
                column += 2;
            }
            continue; // Skip all content inside comments
        }

        // Check for start of tag
        if (c == '<' && !inTag)
        {
            inTag = true;
            currentTag.clear();
            tagBuffer.clear();
            tagBuffer += c;
            continue;
        }
        // Check for end of tag
        else if (c == '>' && inTag)
        {
            inTag = false;
            tagBuffer += c;

            // Trim the tag content
            std::string trimmedTag = trim(currentTag);

            if (trimmedTag.empty())
            {
                // Empty tag, write as is
                fixedXml += tagBuffer;
                continue;
            }

            // Determine if it's a closing tag
            bool isClosing = false;
            size_t pos = 0;
            if (trimmedTag[0] == '/')
            {
                isClosing = true;
                pos = 1; // Skip '/'
            }

            // Extract tag name
            std::string tagName;
            while (pos < trimmedTag.size() &&
                   !std::isspace(static_cast<unsigned char>(trimmedTag[pos])) &&
                   trimmedTag[pos] != '/')
            {
                tagName += trimmedTag[pos];
                pos++;
            }

            tagName = trim(tagName);

            // Determine if it's a self-closing tag
            bool isSelfClosing = false;
            if (!isClosing && !trimmedTag.empty() && trimmedTag.back() == '/')
            {
                isSelfClosing = true;
            }

            if (isSelfClosing)
            {
                // Self-closing tag; write as is
                fixedXml += tagBuffer;
                continue;
            }

            if (isClosing)
            {
                if (tagStack.empty())
                {
                    // Unexpected closing tag -> skip writing it
                    // Do not append tagBuffer
                    continue;
                }
                else
                {
                    if (tagStack.top() == tagName)
                    {
                        // Perfect match; write the closing tag
                        fixedXml += tagBuffer;
                        tagStack.pop();
                    }
                    else
                    {
                        // Mismatched tag
                        // Insert the expected closing tag first
                        fixedXml += "</" + tagStack.top() + ">";
                        tagStack.pop();
                        // Do not write the mismatched closing tag
                        // This prevents duplicate errors
                    }
                }
            }
            else
            {
                // Opening tag -> push to stack and write out
                tagStack.push(tagName);
                fixedXml += tagBuffer;
            }
        }
        else if (inTag)
        {
            // Build up the tag content (including '/', attributes, etc.)
            currentTag += c;
            tagBuffer += c;
        }
        else
        {
            // Outside of a tag, just copy the character to fixedXml
            fixedXml += c;
        }
    }

    // Close any unclosed tags
    while (!tagStack.empty())
    {
        fixedXml += "</" + tagStack.top() + ">";
        tagStack.pop();
    }

    // Write the fixedXml to the specified output file
    std::ofstream outFile(outputPath);
    if (!outFile)
    {
        std::cerr << "Cannot open output file: " << outputPath << std::endl;
        return;
    }
    outFile << fixedXml;
    outFile.close();
}
