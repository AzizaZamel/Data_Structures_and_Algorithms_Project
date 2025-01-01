#ifndef XML_CHECKER_H
#define XML_CHECKER_H

#include <string>

class XmlChecker
{
public:
    // Check XML consistency:
    // - Prints line/column-based errors to std::cerr.
    // - Prints "valid" if no errors, otherwise "invalid" and total error count.
    // - Returns the number of errors detected.
    int checkConsistency(const std::string &xmlContent);

    // Fix XML content and write corrected version to outputPath
    void fixXml(const std::string &xmlContent, const std::string &outputPath);

protected:
    // Helper to report errors with line and column numbers
    virtual void reportError(const std::string &message, int line, int column);
};

#endif
