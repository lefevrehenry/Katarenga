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

    ClientRegistry& registry = m_server->client_registry();

    using ClientId = ClientRegistry::ClientId;
    using ClientSocket = ClientRegistry::ClientSocket;

    std::string name = request.name;
    std::string ip = request.ip;
    std::string port = request.port;

    ClientId id = ClientRegistry::Id(request.ip, request.port);

    if(registry.client_exists(id)) {
    } else {
        registry.add_client(id);

        ClientSocket socket = registry.socket(id);
        //socket.reset(new PlayerSocket());
    }

//    switch (action) {
//    case GameAction::Create:
//        // creer la game et fournir l'id
//        break;
//    case GameAction::Join:
//        break;
//    case GameAction::Spectate:
//        break;
//    default:
//    }

    // 1. verifier si le client n'est pas déjà dans la base de donnee
    // 2. l'ajouter et ouvrir une pair socket
    // 3. répondre que c'est ok et lui communiquer le nouveau port de communication

    reply.status = "connection ok";

    return reply;
}
