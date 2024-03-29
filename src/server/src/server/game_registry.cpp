#include "game_registry.hpp"

GameRegistry::GameRegistry() :
    m_games()
{
}

Game::SPtr GameRegistry::find_game(GameId id) const
{
    if(!game_exists(id))
        return nullptr;

    return game(id);
}

bool GameRegistry::game_exists(GameId id) const
{
    return m_games.find(id) != m_games.end();
}

bool GameRegistry::add_game(GameId id, const Game::SPtr& game)
{
    if(game_exists(id) || !game)
        return false;

    m_games.insert(std::make_pair(id, game));

    return true;
}

bool GameRegistry::remove_game(GameId id)
{
    if(!game_exists(id))
        return false;

    m_games.erase(id);

    return true;
}

Game::SPtr GameRegistry::game(GameRegistry::GameId id) const
{
    if(!game_exists(id))
        throw std::runtime_error("game doesn't exist");

    return m_games.at(id);
}
