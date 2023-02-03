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
    Game::SPtr game = m_client->game();

    if(game)
    {
        game->set_server_socket(nullptr);

        m_client->destroy_game();
    }

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
void ServerSocket::execute_receive_message<GameCreated>(const typename GameCreated::Parameters& p)
{
    m_client->destroy_game();

    m_client->create_game(p.actor);

    Game::SPtr game = m_client->game();

    if(game)
    {
        msg_client("Game created");
//        std::cout << p.position << std::endl;
//        m_game->init_from_position();

        ServerSocket::SPtr socket = shared_from_this();

        game->set_server_socket(socket);
    }

    // TODO: init le board
}

template<>
void ServerSocket::execute_receive_message<GameJoined>(const typename GameJoined::Parameters& p)
{
    // TODO
}

template<>
void ServerSocket::execute_receive_message<GameSpectated>(const typename GameSpectated::Parameters& p)
{
    // TODO
}

template<>
void ServerSocket::execute_receive_message<PlayMove::Reply>(const typename PlayMove::Reply::Parameters& p)
{
    Game::SPtr game = m_client->game();

    if(game && p.accepted) {
        Common::Move m = p.move;

        bool ok = game->play(m);

        if(ok)
            msg_client("move has been played");
        else
            msg_client("server is not synchronized");
    }
    else {
        msg_client("move has been rejected");
    }
}
