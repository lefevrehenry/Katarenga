#include "player_socket.hpp"

// Katarenga
#include <server/server.hpp>

PlayerSocket::PlayerSocket(Server* server, zmqpp::context *context, const zmqpp::endpoint_t &endpoint) :
    PairSocket(context, endpoint),
    m_server(server)
{
}
