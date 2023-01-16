#ifndef KATARENGA_SERVER_SOCKETS_CONNECTION_SOCKET_HPP
#define KATARENGA_SERVER_SOCKETS_CONNECTION_SOCKET_HPP

// Katarenga
#include <common/sockets/reply_socket.hpp>
#include <server/server.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

/**
 * @brief The ConnectionSocket class
 */
class ConnectionSocket : public ReplySocket<ConnectionSocket>
{
public:
    ConnectionSocket(Server* server, zmqpp::context* context, const zmqpp::endpoint_t& endpoint);

public:
    template< typename M >
    typename M::Reply::Parameters execute_message(const typename M::Request::Parameters& parameters) {
        // simply forward the message
        return m_server->template execute_message<M>(parameters);
    }

private:
    Server* m_server;
};

#endif // KATARENGA_SERVER_SOCKETS_CONNECTION_SOCKET_HPP
