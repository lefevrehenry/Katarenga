#include "connection_socket.hpp"

// Katarenga
#include <common/messages/messages.hpp>

#include <server/server.hpp>
#include <server/client_registry.hpp>

ConnectionSocket::ConnectionSocket(Server* server, zmqpp::context *context, const zmqpp::endpoint_t &endpoint) :
    ReplySocket(context, endpoint),
    m_server(server)
{
    bind(endpoint);

    registerMessage<NewConnection>();
    registerMessage<Ping>();
}

template<>
typename NewConnection::Reply::Parameters ConnectionSocket::execute_message<NewConnection>(const typename NewConnection::Request::Parameters& request)
{
    typename NewConnection::Reply::Parameters reply;

    reply.accepted = false;
    std::strncpy(reply.pair_endpoint, "", sizeof(reply.pair_endpoint));

    ClientRegistry* registry = m_server->client_registry();

    //std::string name = request.name;
    //std::string ip = request.ip;
    //std::string port = request.port;

    ClientRegistry::ClientId id = ClientRegistry::Id(request.ip, request.port);

    if(registry->client_exists(id)) {
        ClientRegistry::ClientSocket::SPtr socket = registry->socket(id);

        reply.accepted = true;
        std::strncpy(reply.pair_endpoint, socket->endpoint().c_str(), sizeof(reply.pair_endpoint));

        return reply;
    }

    zmqpp::endpoint_t endpoint = m_server->create_new_client_endpoint();

    if(endpoint.length() > sizeof(reply.pair_endpoint)-1)
        return reply;

    ClientRegistry::ClientSocket::SPtr socket(new ClientRegistry::ClientSocket(m_server, context(), endpoint));

    if(!registry->add_client(id, socket))
        return reply;

    reply.accepted = true;
    std::strncpy(reply.pair_endpoint, socket->endpoint().c_str(), sizeof(reply.pair_endpoint));

    return reply;
}

template<>
typename Ping::Reply::Parameters ConnectionSocket::execute_message<Ping>(const typename Ping::Request::Parameters&)
{
    return {};
}
