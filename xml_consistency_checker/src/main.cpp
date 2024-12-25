// #include "xml_checker.hpp"
// #include <fstream>
// #include <sstream>
// #include <iostream>

// int main(int argc, char *argv[])
// {
//     if (argc < 2)
//     {
//         std::cerr << "Usage: " << argv[0] << " <xml_file> [-f -o output.xml]" << std::endl;
//         return 1;
//     }

//     // Read entire XML file into a string
//     std::ifstream file(argv[1]);
//     if (!file)
//     {
//         std::cerr << "Cannot open XML file: " << argv[1] << std::endl;
//         return 1;
//     }
//     std::stringstream buffer;
//     buffer << file.rdbuf();
//     std::string xmlContent = buffer.str();

//     XmlChecker checker;

//     // Always check consistency first
//     checker.checkConsistency(xmlContent);

//     // If user asked to fix the file: e.g., -f -o output.xml
//     bool fixRequested = false;
//     std::string outputPath;
//     for (int i = 2; i < argc; i++)
//     {
//         std::string arg = argv[i];
//         if (arg == "-f")
//         {
//             fixRequested = true;
//         }
//         else if (arg == "-o" && i + 1 < argc)
//         {
//             outputPath = argv[++i];
//         }
//     }

//     if (fixRequested && !outputPath.empty())
//     {
//         checker.fixXml(xmlContent, outputPath);
//         std::cout << "Fixed XML written to: " << outputPath << std::endl;
//     }

//     return 0;
// }