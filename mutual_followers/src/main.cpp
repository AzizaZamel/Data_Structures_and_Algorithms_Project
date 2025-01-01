#include <iostream>
#include <memory>
#include <map>
#include <set>
#include "xml_parser.hpp"
#include "xml_node.hpp"
#include "mutual_followers.hpp"
#include "user.hpp"
#include "utils.hpp"

int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        std::cerr << "Usage: " << argv[0] << " mutual -i <users_xml> -ids 1,2,3" << std::endl;
        return 1;
    }

    std::string filename, ids_str;
    bool foundMutual = false;

    // Parse CLI arguments
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "mutual")
        {
            foundMutual = true;
        }
        else if (arg == "-i")
        {
            if (++i < argc)
                filename = argv[i];
        }
        else if (arg == "-ids")
        {
            if (++i < argc)
                ids_str = argv[i];
        }
    }

    if (!foundMutual || filename.empty() || ids_str.empty())
    {
        log_error("Command must specify 'mutual', '-i file.xml', and '-ids <list>'.");
        return 1;
    }

    auto user_ids = parse_ids(ids_str);
    if (user_ids.empty())
    {
        log_error("No valid user IDs provided.");
        return 1;
    }

    // Get mutual followers as a string
    std::string result = get_mutual_followers_string(user_ids, filename);

    // Display the result
    std::cout << result;

    return 0;
}
