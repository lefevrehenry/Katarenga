#include "connection_socket.hpp"

ConnectionSocket::ConnectionSocket(zmqpp::context *context, const zmqpp::endpoint_t &endpoint) :
    ReplySocket(context, endpoint)
{
}
