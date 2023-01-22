#include "player_socket.hpp"

// Katarenga
#include <common/messages/messages.hpp>
#include <server/server.hpp>

PlayerSocket::PlayerSocket(Server* server, zmqpp::context *context, const zmqpp::endpoint_t &endpoint) :
    PairSocket(context, endpoint),
    m_server(server)
{
    registerMessage<CreateGame>();
    registerMessage<JoinGame>();
    registerMessage<SpectateGame>();
}

//template<>
//void PlayerSocket::execute_receive_message<MovePlayer>(const typename MovePlayer::Parameters&)
//{
//}

//template<>
//void PlayerSocket::execute_send_message<MovePlayer>(typename MovePlayer::Parameters*)
//{
//}
