#include "player_socket.hpp"

// Katarenga
#include <common/messages/messages.hpp>
#include <server/game_registry.hpp>
#include <server/server.hpp>

PlayerSocket::PlayerSocket(Server* server, zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
    PairSocket(context, endpoint),
    std::enable_shared_from_this<PlayerSocket>(),
    m_server(server)
{
    registerMessage<CreateGame>();
    registerMessage<JoinGame>();
    registerMessage<SpectateGame>();
}

template<>
void PlayerSocket::execute_receive_message<CreateGame>(const typename CreateGame::Parameters&)
{
    GameRegistry* registry = m_server->game_registry();

    GameRegistry::GameId id = Game::GenerateId();
    Game::SPtr game = Game::Create();

    PlayerSocket::SPtr sp = shared_from_this();
    game->set_white_socket(sp);

    registry->add_game(id, game);

    // 1. creer la game
    // 2. setter le joueur en blanc
    // 3. ajouter la game au registre
    // 4. envoyer en réponse le GameId à travers la socket

    GameCreated::Parameters parameters = { id };
    send_message<GameCreated>(parameters);
}

//template<>
//void PlayerSocket::execute_send_message<MovePlayer>(typename MovePlayer::Parameters*)
//{
//}
