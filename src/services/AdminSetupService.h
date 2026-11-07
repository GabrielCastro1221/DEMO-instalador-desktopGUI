#pragma once
#include <string>

class AdminSetupService
{
public:
    bool createInitialAdmin(const std::string &name, const std::string &user, const std::string &password, std::string &error);
};
