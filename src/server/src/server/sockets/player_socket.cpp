#include "player_socket.hpp"

// Katarenga
#include <common/common_utils.hpp>
#include <common/messages/messages.hpp>
#include <server/game_registry.hpp>
#include <server/server.hpp>

PlayerSocket::PlayerSocket(Server* server, zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
    PairSocket(context, endpoint),
    std::enable_shared_from_this<PlayerSocket>(),
    m_server(server),
    m_is_busy(false)
{
    registerMessage<CreateGame>();
    registerMessage<JoinGame>();
    registerMessage<SpectateGame>();
}

bool PlayerSocket::busy() const
{
    return m_is_busy;
}

template<>
void PlayerSocket::execute_receive_message<CreateGame>(const typename CreateGame::Parameters& p)
{
    GameCreated::Parameters reply;
    reply.accepted = false;
    reply.id = 0;

    // the socket is not already busy by a game ?
    if(!busy())
    {
        GameRegistry* registry = m_server->game_registry();

        GameRegistry::GameId id = Game::GenerateId();
        Game::SPtr game = Game::Create();

        PlayerSocket::SPtr sp = shared_from_this();

        switch (p.actor) {
        case Common::GameActor::White:
            game->set_white_socket(sp);
            break;
        case Common::GameActor::Black:
            game->set_black_socket(sp);
            break;
        default:
            game->set_white_socket(sp);
        }

        registry->add_game(id, game);

        reply.accepted = true;
        reply.id = id;
    }

    send_message<GameCreated>(reply);
}

template<>
void PlayerSocket::execute_receive_message<JoinGame>(const typename JoinGame::Parameters& p)
{
    GameJoined::Parameters reply;
    reply.accepted = false;

    GameRegistry* registry = m_server->game_registry();

    Game::SPtr game = registry->game(p.id);

    // game exists obviously ?
    if(game)
    {
        // is game waiting for players ?
        if(game->is_pending())
        {
            // the socket is not already busy by a game ?
            if(!busy())
            {
                PlayerSocket::SPtr sp = shared_from_this();

                if(game->has_white_player())
                    game->set_black_socket(sp);
                else if(game->has_black_player())
                    game->set_white_socket(sp);

                reply.accepted = true;
            }
        }
    }

    send_message<GameJoined>(reply);
}

template<>
void PlayerSocket::execute_receive_message<SpectateGame>(const typename SpectateGame::Parameters& p)
{
    GameSpectated::Parameters reply;
    reply.accepted = false;

    GameRegistry* registry = m_server->game_registry();

    Game::SPtr game = registry->game(p.id);

    // game exists obviously ?
    if(game)
    {
        // TODO: do it
    }

    send_message<GameSpectated>(reply);
}
