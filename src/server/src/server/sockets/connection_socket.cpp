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
    ClientRegistry& registry = m_server->client_registry();

    // 1. verifier si le client n'est pas déjà dans la base de donnee
    // 2. l'ajouter et ouvrir une pair socket
    // 3. répondre que c'est ok et lui communiquer le nouveau port de communication
    typename NewConnection::Reply::Parameters reply;

    reply.ok = "status";

    return reply;
}
