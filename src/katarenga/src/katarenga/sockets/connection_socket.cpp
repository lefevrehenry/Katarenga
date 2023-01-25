#include "connection_socket.hpp"

// Katarenga
#include <common/messages/messages.hpp>
#include <katarenga/client.hpp>

ConnectionSocket::ConnectionSocket(Client* client, zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
    RequestSocket(context, endpoint),
    m_client(client)
{
    connect(endpoint);

    registerMessage<NewConnection>();
}

template<>
typename NewConnection::Request::Parameters ConnectionSocket::execute_request_message<NewConnection>()
{
    typename NewConnection::Request::Parameters parameters;

    parameters.name = "Jacky";
    parameters.ip = "localhost";
    parameters.port = "28000";

    return parameters;
}

template<>
void ConnectionSocket::execute_reply_message<NewConnection>(const typename NewConnection::Reply::Parameters& reply)
{
}
