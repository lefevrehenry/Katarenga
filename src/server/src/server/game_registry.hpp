#ifndef KATARENGA_SERVER_GAME_REGISTRY_HPP
#define KATARENGA_SERVER_GAME_REGISTRY_HPP

// Katarenga
#include <server/server_utils.hpp>

/**
 * @brief The GameRegistry class
 */
class GameRegistry
{
public:
    using GameId = ServerCommon::GameId;

    static GameId Id()
    {
        return GameId();
    }

public:
    GameRegistry();

    GameRegistry(const GameRegistry&) = delete;
    GameRegistry& operator=(const GameRegistry& other) = delete;

    GameRegistry(GameRegistry&&) = delete;
    GameRegistry& operator=(GameRegistry&& other) = delete;

};

#endif // KATARENGA_SERVER_GAME_REGISTRY_HPP
