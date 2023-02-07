#include "connection_socket.hpp"

// Katarenga
#include <common/messages/messages.hpp>
#include <katarenga/client.hpp>
#include <katarenga/client_utils.hpp>

ConnectionSocket::ConnectionSocket(Client* client, zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
    RequestSocket(context, endpoint),
    m_client(client)
{
    connect(endpoint);

    registerMessage<NewConnection>();
    registerMessage<Ping>();
}

template<>
typename Ping::Request::Parameters ConnectionSocket::execute_request_message<Ping>()
{
//    msg_client("client ping");

    return {};
}

template<>
void ConnectionSocket::execute_reply_message<Ping>(const typename Ping::Reply::Parameters&)
{
//    msg_client("server pong");
}

template<>
typename NewConnection::Request::Parameters ConnectionSocket::execute_request_message<NewConnection>()
{
    typename NewConnection::Request::Parameters parameters;

    std::strncpy(parameters.name, "Jacky", sizeof(parameters.name));
    std::strncpy(parameters.ip, "localhost", sizeof(parameters.ip));
    std::strncpy(parameters.port, "28000", sizeof(parameters.port));

    return parameters;
}

template<>
void ConnectionSocket::execute_reply_message<NewConnection>(const typename NewConnection::Reply::Parameters& reply)
{
    bool accepted = reply.accepted;

    if(!accepted)
    {
        msg_client("connection refused");
        return;
    }

    zmqpp::endpoint_t endpoint = reply.pair_endpoint;

    m_client->open_server_socket(endpoint);

    msg_client("connection etablished (" + endpoint + ")");
}
