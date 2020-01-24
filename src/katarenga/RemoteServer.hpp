#ifndef REMOTESERVER_HPP
#define REMOTESERVER_HPP

#include "AbstractServer.hpp"
#include <message/MessageWrapper.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <string>


/**
 * @brief The RemoteServer class
 */
class RemoteServer : public AbstractServer
{
    using MessageType = MessageWrapper::MessageType;

public:
    RemoteServer(const std::string& server_ip, const std::string& server_port);
    virtual ~RemoteServer();

public:
    bool checkServerConnectivity() override;
    std::string getBoardConfiguration() override;
    bool playThisMove(const std::string&) override;

private:
    template< typename T >
    void send(typename T::Request* request);

    template< typename T >
    void receive(typename T::Reply* reply);

    template< typename T, typename ... Args >
    typename T::Reply communicate(Args&& ...);

private:
    static constexpr unsigned int TimeOut = 5000;     // timeout for the reply of the server (in milliseconds)

private:
    zmqpp::context  m_context;
    zmqpp::socket   m_socket;
    zmqpp::poller   m_poller;

};

#endif
