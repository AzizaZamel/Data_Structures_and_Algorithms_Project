#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>

struct User
{
    int id;
    std::string name;
    std::vector<int> followers;
};

#endif // USER_HPP
