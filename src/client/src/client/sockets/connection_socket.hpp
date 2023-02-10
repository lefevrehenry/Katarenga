#ifndef KATARENGA_CLIENT_SOCKETS_CONNECTION_SOCKET_HPP
#define KATARENGA_CLIENT_SOCKETS_CONNECTION_SOCKET_HPP

// Katarenga
#include <common/messages/message.hpp>
#include <common/sockets/abstract_socket.hpp>
#include <common/sockets/request_socket.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <memory>

class Client;

/**
 * @brief The ConnectionSocket class (client side)
 */
class ConnectionSocket : public RequestSocket<ConnectionSocket>
{
public:
    using SPtr = std::shared_ptr<ConnectionSocket>;

public:
    ConnectionSocket(Client* client, zmqpp::context* context, const zmqpp::endpoint_t& endpoint);

public:
    template< typename M >
    typename M::Request::Parameters execute_request_message() {
        throw std::runtime_error("No implementation yet");
    }

    template< typename M >
    void execute_reply_message(const typename M::Reply::Parameters& reply) {
        throw std::runtime_error("No implementation yet");
    }

private:
    Client* m_client;
};

#endif // KATARENGA_CLIENT_SOCKETS_CONNECTION_SOCKET_HPP
