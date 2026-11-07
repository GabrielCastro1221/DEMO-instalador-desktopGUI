#include "DatabaseSetupService.h"

bool DatabaseSetupService::testConnection(std::string &error)
{
    error = "Conexión MySQL no implementada en la demo.";
    return false;
}
bool DatabaseSetupService::initializeSchema(std::string &error)
{
    error = "Inicialización de esquema no implementada en la demo.";
    return false;
}
