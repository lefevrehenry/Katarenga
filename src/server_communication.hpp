#ifndef SERVER_COMMUNICATION_HPP
#define SERVER_COMMUNICATION_HPP

#include "MessageWrapper.hpp"

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <string>


class AbstractServer
{

public:
    virtual bool checkServerConnectivity() = 0;
    virtual std::string getBoardConfiguration() = 0;

};

class RemoteServer : public AbstractServer
{
    using MessageType = MessageWrapper::MessageType;

public:
    RemoteServer(const std::string& server_ip, const std::string& server_port);
    virtual ~RemoteServer();

private:
    template< typename T >
    void send(typename T::Request* request);

    template< typename T >
    void receive(typename T::Reply* reply);

    // todo implement with a poller to have a timeout
    // todo use perfect forwarding to construct Request
    template< typename T >
    typename T::Reply communicate();

public:
    bool checkServerConnectivity() override;
    std::string getBoardConfiguration() override;

private:
    zmqpp::context  m_context;
    zmqpp::socket   m_socket;

};

class LocalServer : public AbstractServer
{

public:
    LocalServer();

public:
    bool checkServerConnectivity() override;
    std::string getBoardConfiguration() override;

private:
//    zmqpp::context  m_context;

};

#endif  // SERVER_COMMUNICATION_HPP
