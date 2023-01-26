#ifndef KATARENGA_CLIENT_GAME_HPP
#define KATARENGA_CLIENT_GAME_HPP

// Katarenga
//#include <server/server_utils.hpp>
//#include <server/sockets/player_socket.hpp>

// Nod
//#include <nod/nod.hpp>

// Standard Library
#include <memory>

class Game
{
public:
    using SPtr = std::shared_ptr<Game>;

public:
    Game();

//public:
//    bool is_pending() { return m_status == Status::Pending; }
//    bool is_running() { return m_status == Status::Running; }
//    bool is_over() { return m_status == Status::Finished; }

//    bool has_white_player() const;
//    bool has_black_player() const;

//public:
//    void set_white_socket(const PlayerSocket::SPtr& socket);
//    void set_black_socket(const PlayerSocket::SPtr& socket);

//public:
//    nod::signal<void()> white_player_joined;
//    nod::signal<void()> white_player_left;

//    nod::signal<void()> black_player_joined;
//    nod::signal<void()> black_player_left;

//private:
//    void update_status();

private:
//    Status m_status;

//    ServerSocket::SPtr m_server_socket;

};

#endif // KATARENGA_CLIENT_GAME_HPP
