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

    template< typename T, typename ... Args >
    typename T::Reply communicate(Args&& ...);

private:
    static constexpr unsigned int TimeOut = 5000;     // timeout in milliseconds

public:
    bool checkServerConnectivity() override;
    std::string getBoardConfiguration() override;

private:
    zmqpp::context  m_context;
    zmqpp::socket   m_socket;
    zmqpp::poller   m_poller;

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
