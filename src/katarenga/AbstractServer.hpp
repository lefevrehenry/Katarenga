#ifndef ABSTRACTSERVER_HPP
#define ABSTRACTSERVER_HPP

// Standard Library
#include <string>


/**
 * @brief The AbstractServer class
 */
class AbstractServer
{

public:
    AbstractServer() = default;

public:
    virtual bool checkServerConnectivity() = 0;
    virtual std::string getBoardConfiguration() = 0;
    virtual bool playThisMove(const std::string&) = 0;

};

#endif
