#ifndef KATARENGA_SERVER_SOCKETS_CONNECTION_SOCKET_HPP
#define KATARENGA_SERVER_SOCKETS_CONNECTION_SOCKET_HPP

// Katarenga
#include <common/sockets/reply_socket.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

class Server;

/**
 * @brief The ConnectionSocket class (server side)
 */
class ConnectionSocket : public ReplySocket<ConnectionSocket>
{
public:
    ConnectionSocket(Server* server, zmqpp::context* context, const zmqpp::endpoint_t& endpoint);

public:
    template< typename M >
    typename M::Reply::Parameters execute_message(const typename M::Request::Parameters& parameters) {
        throw std::runtime_error("No implementation yet");
    }

private:
    Server* m_server;

};

#endif // KATARENGA_SERVER_SOCKETS_CONNECTION_SOCKET_HPP
