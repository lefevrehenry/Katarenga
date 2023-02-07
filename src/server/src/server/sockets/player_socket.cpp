#include "player_socket.hpp"

// Katarenga
#include <common/messages/messages.hpp>
#include <server/game_registry.hpp>
#include <server/server.hpp>

PlayerSocket::PlayerSocket(Server* server, zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
    PairSocket(context, endpoint),
    std::enable_shared_from_this<PlayerSocket>(),
    m_server(server),
    m_is_busy(false),
    m_id(GameId()),
    m_actor(GameActor::None)
{
    bind(endpoint);

    registerReceiveMessage<CloseConnection>();

    registerReceiveMessage<CreateGame>();
    registerReceiveMessage<JoinGame>();
    registerReceiveMessage<SpectateGame>();

    registerReceiveMessage<PlayMove::Request>();

    registerSendMessage<GameCreated>();
    registerSendMessage<GameJoined>();
    registerSendMessage<GameSpectated>();

    registerSendMessage<PlayMove::Reply>();
}

template<>
void PlayerSocket::execute_receive_message<CloseConnection>(const typename CloseConnection::Parameters& p)
{
    PlayerSocket::SPtr socket = shared_from_this();

    ClientRegistry* registry = m_server->client_registry();
    ClientRegistry::ClientId id = registry->id(socket);

    if(id == ClientRegistry::ClientId()) {
        msg_server("PlayerSocket id not registered");
        return;
    }

    registry->remove_client(id);
}

bool PlayerSocket::busy() const
{
    return m_is_busy;
}

template<>
void PlayerSocket::execute_receive_message<CreateGame>(const typename CreateGame::Parameters& p)
{
    m_id = GameId();
    m_actor = GameActor::None;

    // the socket is not already busy by a game ?
    if(!busy())
    {
        GameRegistry* registry = m_server->game_registry();

        GameRegistry::GameId id = Game::GenerateId();
        Game::SPtr game = Game::Create();

        PlayerSocket::SPtr socket = shared_from_this();

        switch (p.actor) {
        case GameActor::White:
            game->set_white_socket(socket);
            break;
        case GameActor::Black:
            game->set_black_socket(socket);
            break;
        default:
            game->set_white_socket(socket);
        }

        registry->add_game(id, game);

        m_id = id;
        m_actor = p.actor;
    }

    send_message<GameCreated>();
}

template<>
void PlayerSocket::execute_receive_message<JoinGame>(const typename JoinGame::Parameters& p)
{
    GameRegistry* registry = m_server->game_registry();

    Game::SPtr game = registry->find_game(p.id);

    // game exists obviously ?
    if(game)
    {
        PlayerSocket::SPtr socket = shared_from_this();

        // if player is already in the game
        if(game->is_white_player(socket)) {
            m_id = p.id;
            m_actor = GameActor::White;
        } else if(game->is_black_player(socket)) {
            m_id = p.id;
            m_actor = GameActor::Black;
        }
        // otherwise is game waiting for players ?
        else if(game->is_pending())
        {
            // the socket is not already busy by a game ?
            if(!busy())
            {
                if(game->has_white_player()) {
                    game->set_black_socket(socket);
                    m_id = p.id;
                    m_actor = GameActor::Black;
                } else if(game->has_black_player()) {
                    game->set_white_socket(socket);
                    m_id = p.id;
                    m_actor = GameActor::White;
                } else {
                    m_id = GameId();
                    m_actor = GameActor::None;
                }
            }
        }
    }

    send_message<GameJoined>();
}

template<>
void PlayerSocket::execute_receive_message<SpectateGame>(const typename SpectateGame::Parameters& p)
{
    GameRegistry* registry = m_server->game_registry();

    Game::SPtr game = registry->game(p.id);

    // game exists obviously ?
    if(game)
    {
        // TODO: do it
    }

    send_message<GameSpectated>();
}

template<>
void PlayerSocket::execute_receive_message<PlayMove::Request>(const typename PlayMove::Request::Parameters& p)
{
    typename PlayMove::Reply::Parameters reply;
    reply.accepted = false;
    reply.move = Common::Move();

    GameRegistry* registry = m_server->game_registry();

    Game::SPtr game = registry->game(m_id);

    // game exists obviously ?
    if(game)
    {
        Common::Move move = p.move;

        if(game->play(move, m_actor))
        {
            // confirm the move has been played
            reply.accepted = true;
            reply.move = move;
        }
    }

    // by-pass the engine
    send_message<PlayMove::Reply>(reply);
}

template<>
typename GameCreated::Parameters PlayerSocket::execute_send_message<GameCreated>()
{
    GameCreated::Parameters p;
    p.accepted = false;

    GameRegistry* registry = m_server->game_registry();
    Game::SPtr game = registry->find_game(m_id);

    if(game)
    {
        const std::string& position = game->position();

        p.accepted = true;
        p.id = m_id;
        p.actor = m_actor;
        std::strncpy(p.position, position.c_str(), sizeof(p.position));
    }

    return p;
}

template<>
typename GameJoined::Parameters PlayerSocket::execute_send_message<GameJoined>()
{
    GameJoined::Parameters p;
    p.accepted = false;

    GameRegistry* registry = m_server->game_registry();
    Game::SPtr game = registry->find_game(m_id);

    if(game)
    {
        const std::string& position = game->position();

        p.accepted = true;
        p.id = m_id;
        p.actor = m_actor;
        std::strncpy(p.position, position.c_str(), sizeof(p.position));
    }

    return p;
}

template<>
typename GameSpectated::Parameters PlayerSocket::execute_send_message<GameSpectated>()
{
    GameSpectated::Parameters p;

    p.accepted = false;

    return p;
}

template<>
typename PlayMove::Reply::Parameters PlayerSocket::execute_send_message<PlayMove::Reply>()
{
    return {};
}
