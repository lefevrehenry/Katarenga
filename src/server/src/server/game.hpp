#ifndef KATARENGA_SERVER_GAME_HPP
#define KATARENGA_SERVER_GAME_HPP

// Katarenga
#include <server/server_utils.hpp>
#include <server/sockets/player_socket.hpp>

// Standard Library
#include <memory>

class Game
{
public:
    using SPtr = std::shared_ptr<Game>;

public:
    Game();

public:
    static ServerCommon::GameId GenerateId();
    static Game::SPtr Create();

public:
    void set_white_socket(const PlayerSocket::SPtr& socket);
    void set_black_socket(const PlayerSocket::SPtr& socket);

private:
    PlayerSocket::SPtr m_white_socket;
    PlayerSocket::SPtr m_black_socket;

};

#endif // KATARENGA_SERVER_GAME_HPP
