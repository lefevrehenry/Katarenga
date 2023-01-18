#include "connection_socket.hpp"

// Katarenga
#include <server/server.hpp>
#include <server/client_registry.hpp>

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

    reply.accepted = false;
    reply.pair_endpoint = "";

    ClientRegistry* registry = m_server->client_registry();

    using ClientId = ClientRegistry::ClientId;
    using ClientSocket = ClientRegistry::ClientSocket;
    using ClientEndpoint = ClientRegistry::ClientEndpoint;

    //std::string name = request.name;
    std::string ip = request.ip;
    std::string port = request.port;

    ClientId id = ClientRegistry::Id(request.ip, request.port);

    if(registry->client_exists(id))
        return reply;

    registry->add_client(id);

    //ClientSocket socket = registry->socket(id);
    ClientEndpoint endpoint = registry->endpoint(id);

    reply.accepted = true;
    reply.pair_endpoint = endpoint.c_str();

    return reply;
}
