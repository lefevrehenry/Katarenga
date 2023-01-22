#include "game.hpp"

Game::Game() :
    m_white_socket(),
    m_black_socket()
{
}

ServerCommon::GameId Game::GenerateId()
{
    static ServerCommon::GameId _id = 1;
    return _id++;
}

Game::SPtr Game::Create()
{
    return std::make_shared<Game>();
}

void Game::set_white_socket(const PlayerSocket::SPtr& socket)
{
    if(!socket)
        throw std::runtime_error("socket is null");

    m_white_socket = socket;
}

void Game::set_black_socket(const PlayerSocket::SPtr& socket)
{
    if(!socket)
        throw std::runtime_error("socket is null");

    m_black_socket = socket;
}
