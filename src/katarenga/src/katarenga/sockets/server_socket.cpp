#include "server_socket.hpp"

// Katarenga
#include <common/messages/messages.hpp>

#include <katarenga/client.hpp>
#include <katarenga/client_utils.hpp>
#include <katarenga/game.hpp>

ServerSocket::ServerSocket(Client* client, zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
    PairSocket(context, endpoint),
    std::enable_shared_from_this<ServerSocket>(),
    m_client(client)
{
    connect(endpoint);

    registerSendMessage<CreateGame>();
    registerSendMessage<JoinGame>();
    registerSendMessage<SpectateGame>();

    registerReceiveMessage<GameCreated>();
    registerReceiveMessage<GameJoined>();
    registerReceiveMessage<GameSpectated>();
}

template<>
void ServerSocket::execute_receive_message<GameCreated>(const typename GameCreated::Parameters& p)
{
    msg_client("GameCreated " + std::to_string(p.accepted) + " | " + std::to_string(p.id));

    Game::SPtr game = m_client->create_game();

    // TODO: init le board
}

template<>
void ServerSocket::execute_receive_message<GameJoined>(const typename GameJoined::Parameters& p)
{
}

template<>
void ServerSocket::execute_receive_message<GameSpectated>(const typename GameSpectated::Parameters& p)
{
}
