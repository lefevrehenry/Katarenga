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

    m_server->close_connection(socket);
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

    {
        GameId id = m_server->create_game(p.actor, shared_from_this());

        m_id = id;
        m_actor = p.actor;
    }

    send_message<GameCreated>();
}

template<>
void PlayerSocket::execute_receive_message<JoinGame>(const typename JoinGame::Parameters& p)
{
    m_server->join_game(p.id, shared_from_this());

    send_message<GameJoined>();
}

template<>
void PlayerSocket::execute_receive_message<SpectateGame>(const typename SpectateGame::Parameters& p)
{
    m_server->spectate_game(p.id, shared_from_this());

    send_message<GameSpectated>();
}

template<>
void PlayerSocket::execute_receive_message<PlayMove::Request>(const typename PlayMove::Request::Parameters& p)
{
    typename PlayMove::Reply::Parameters reply;

    if(m_server->play_move(m_id, p.move, m_actor))
    {
        reply.accepted = true;
        reply.move = p.move;
    }
    else
    {
        reply.accepted = false;
        reply.move = Common::Move();
    }

    // /!\ by-pass the engine
    send_message<PlayMove::Reply>(reply);
}

template<>
typename GameCreated::Parameters PlayerSocket::execute_send_message<GameCreated>()
{
    GameCreated::Parameters p;

    const std::string& position = m_server->game_position(m_id);

    if(!position.empty())
    {
        p.accepted = true;
        p.id = m_id;
        p.actor = m_actor;
        std::strncpy(p.position, position.c_str(), sizeof(p.position));
    }
    else
    {
        p.accepted = false;
        p.id = GameId();
        p.actor = GameActor::None;
        std::strncpy(p.position, "", sizeof(p.position));
    }

    return p;
}

template<>
typename GameJoined::Parameters PlayerSocket::execute_send_message<GameJoined>()
{
    GameJoined::Parameters p;

    const std::string& position = m_server->game_position(m_id);

    if(!position.empty())
    {
        p.accepted = true;
        p.id = m_id;
        p.actor = m_actor;
        std::strncpy(p.position, position.c_str(), sizeof(p.position));
    }
    else
    {
        p.accepted = false;
        p.id = GameId();
        p.actor = GameActor::None;
        std::strncpy(p.position, "", sizeof(p.position));
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
