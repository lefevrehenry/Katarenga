#include "connection_socket.hpp"

// Katarenga
#include <server/server.hpp>

ConnectionSocket::ConnectionSocket(Server* server, zmqpp::context *context, const zmqpp::endpoint_t &endpoint) :
    ReplySocket(context, endpoint),
    m_server(server)
{
    registerMessage<NewConnection>();
}

template<>
typename NewConnection::Reply::Parameters ConnectionSocket::execute_message<NewConnection>(const typename NewConnection::Request::Parameters& request)
{
    typename NewConnection::Reply::Parameters reply;

    reply.ok = "status";

    return reply;
}
