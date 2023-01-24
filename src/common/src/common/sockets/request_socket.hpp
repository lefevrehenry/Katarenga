#ifndef KATARENGA_COMMON_SOCKETS_REQUEST_SOCKET_HPP
#define KATARENGA_COMMON_SOCKETS_REQUEST_SOCKET_HPP

// Katarenga
#include <common/sockets/abstract_socket.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

/**
 * @brief The RequestSocket class
 */
class RequestSocket : public AbstractSocket
{
public:
    RequestSocket(zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
        AbstractSocket(context, zmqpp::socket_type::request, endpoint)
    {
    }

public:
    void process_input_message() {
    }
};

#endif // KATARENGA_COMMON_SOCKETS_REQUEST_SOCKET_HPP
