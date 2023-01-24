#ifndef KATARENGA_CLIENT_SOCKETS_CONNECTION_SOCKET_HPP
#define KATARENGA_CLIENT_SOCKETS_CONNECTION_SOCKET_HPP

// Katarenga
#include <common/sockets/abstract_socket.hpp>
#include <common/sockets/request_socket.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

class Client;

/**
 * @brief The ConnectionSocket class
 */
class ConnectionSocket : public RequestSocket
{
public:
    ConnectionSocket(Client* client, zmqpp::context* context, const zmqpp::endpoint_t& endpoint);

private:
    Client* m_client;
};

#endif // KATARENGA_CLIENT_SOCKETS_CONNECTION_SOCKET_HPP
