#ifndef KATARENGA_CLIENT_CLIENT_HPP
#define KATARENGA_CLIENT_CLIENT_HPP

// Katarenga
#include <common/common_utils.hpp>

#include <katarenga/sockets/connection_socket.hpp>
#include <katarenga/sockets/server_socket.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

/**
 * @brief The Client class
 */
class Client
{
public:
    Client(const ServerInfo& server_info);

public:
    ConnectionSocket::SPtr connection_socket() const;
    ServerSocket::SPtr server_socket() const;

public:
    void open_server_socket(const zmqpp::endpoint_t& endpoint);
    void close_server_socket();

public:
    int exec();

private:
    // Server-related content
    ServerInfo m_server_info;

    // Socket-related content
    zmqpp::context      m_zmq_context;
    zmqpp::poller       m_poller;

    ConnectionSocket    m_connection_socket;
    ServerSocket::SPtr  m_server_socket;

};

#endif // KATARENGA_CLIENT_CLIENT_HPP
