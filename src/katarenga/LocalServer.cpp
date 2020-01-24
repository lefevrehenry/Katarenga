#include "LocalServer.hpp"

LocalServer::LocalServer() : AbstractServer()
{
}

bool LocalServer::checkServerConnectivity()
{
    return true;
}

std::string LocalServer::getBoardConfiguration()
{
    return  "";
}

bool LocalServer::playThisMove(const std::string&)
{
    return false;
}
