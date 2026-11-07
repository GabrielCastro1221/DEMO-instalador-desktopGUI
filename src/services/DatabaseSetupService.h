#pragma once
#include <string>

class DatabaseSetupService
{
public:
    bool testConnection(std::string &error);
    bool initializeSchema(std::string &error);
};
