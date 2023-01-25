#include "server_socket.hpp"

// Katarenga
#include <common/messages/messages.hpp>

ServerSocket::ServerSocket(Client* client, zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
    PairSocket(context, endpoint),
    std::enable_shared_from_this<ServerSocket>()
{
    connect(endpoint);

    registerMessage<GameCreated>();
    registerMessage<GameJoined>();
    registerMessage<GameSpectated>();
}
