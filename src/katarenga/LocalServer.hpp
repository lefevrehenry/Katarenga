#ifndef LOCALSERVER_HPP
#define LOCALSERVER_HPP

#include "AbstractServer.hpp"

// Standard Library
#include <string>


/**
 * @brief The LocalServer class
 */
class LocalServer : public AbstractServer
{

public:
    LocalServer();

public:
    bool checkServerConnectivity() override;
    std::string getBoardConfiguration() override;
    bool playThisMove(const std::string&) override;

};

#endif
