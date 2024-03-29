#ifndef KATARENGA_SERVER_GAME_REGISTRY_HPP
#define KATARENGA_SERVER_GAME_REGISTRY_HPP

// Katarenga
#include <server/game.hpp>
#include <server/server_utils.hpp>

// Standard Library
#include <map>

/**
 * @brief The GameRegistry class
 */
class GameRegistry
{
    friend class Server;

public:
    using GameId = ServerCommon::GameId;
    using Games = std::map<GameId, Game::SPtr>;

public:
    GameRegistry();

    GameRegistry(const GameRegistry&) = delete;
    GameRegistry& operator=(const GameRegistry& other) = delete;

    GameRegistry(GameRegistry&&) = delete;
    GameRegistry& operator=(GameRegistry&& other) = delete;

public:
    Game::SPtr find_game(GameId id) const;
    bool game_exists(GameId id) const;

    bool add_game(GameId id, const Game::SPtr& game);
    bool remove_game(GameId id);

public:
    Game::SPtr game(GameId id) const;

private:
    Games m_games;

};

#endif // KATARENGA_SERVER_GAME_REGISTRY_HPP
