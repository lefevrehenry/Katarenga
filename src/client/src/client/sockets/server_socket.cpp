#include "server_socket.hpp"

// Katarenga
#include <common/messages/messages.hpp>

#include <client/client.hpp>
#include <client/client_utils.hpp>
#include <client/game.hpp>

ServerSocket::ServerSocket(Client* client, zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
    PairSocket(context, endpoint),
    std::enable_shared_from_this<ServerSocket>(),
    m_client(client)
{
    connect(endpoint);

    registerSendMessage<CloseConnection>();

    registerSendMessage<CreateGame>();
    registerSendMessage<JoinGame>();
    registerSendMessage<SpectateGame>();

    registerSendMessage<typename PlayMove::Request>();

    registerReceiveMessage<GameCreated>();
    registerReceiveMessage<GameJoined>();
    registerReceiveMessage<GameSpectated>();

    registerReceiveMessage<typename PlayMove::Reply>();
}

template<>
typename CloseConnection::Parameters ServerSocket::execute_send_message<CloseConnection>()
{
    m_client->close_connection();

    return {};
}

template<>
typename CreateGame::Parameters ServerSocket::execute_send_message<CreateGame>()
{
    CreateGame::Parameters p;

    p.actor = GameActor::White;

    return p;
}

template<>
typename JoinGame::Parameters ServerSocket::execute_send_message<JoinGame>()
{
    JoinGame::Parameters p;

    // TODO
    p.id = 1;//Common::GameId();

    return p;
}

template<>
void ServerSocket::execute_receive_message<GameCreated>(const typename GameCreated::Parameters& p)
{
    if(p.accepted) {
        m_client->init_game(p.actor, p.position, shared_from_this());

        msg_client("Game created");
    } else {
        msg_client("create game has been rejected");
    }
}

template<>
void ServerSocket::execute_receive_message<GameJoined>(const typename GameJoined::Parameters& p)
{
    if(p.accepted) {
        m_client->init_game(p.actor, p.position, shared_from_this());

        msg_client("Game joined");
    } else {
        msg_client("join game has been rejected");
    }
}

template<>
void ServerSocket::execute_receive_message<GameSpectated>(const typename GameSpectated::Parameters& p)
{
    // TODO
}

template<>
void ServerSocket::execute_receive_message<PlayMove::Reply>(const typename PlayMove::Reply::Parameters& p)
{
    if(p.accepted) {
        bool ok = m_client->play_move(p.move);

        if(ok)
            msg_client("move has been played");
        else
            msg_client("server is not synchronized");
    }
    else {
        msg_client("move has been rejected");
    }
}
