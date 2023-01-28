#ifndef KATARENGA_CLIENT_GAME_HPP
#define KATARENGA_CLIENT_GAME_HPP

// Katarenga
#include <common/board/Board.hpp>
#include <common/common_utils.hpp>

#include <katarenga/sockets/server_socket.hpp>

// Nod
#include <nod/nod.hpp>

// Standard Library
#include <memory>

class Game
{
public:
    using SPtr = std::shared_ptr<Game>;

private:
    using GameActor = Common::GameActor;

    enum class Status {
        Pending,        // waiting for connection with the server
        Running,        // game is running with the server
        Finished        // game is over
    };

public:
    Game(GameActor actor);

public:
    bool is_pending() { return m_status == Status::Pending; }
    bool is_running() { return m_status == Status::Running; }
    bool is_over() { return m_status == Status::Finished; }

    bool has_server_socket() const;

public:
    void set_server_socket(const ServerSocket::SPtr& socket);

public:
    const Board* board() const { return &m_board; }

public:
    nod::signal<void()> server_joined;
    nod::signal<void()> server_left;

private:
    void update_status();

private:
    // board-related content
    Status      m_status;
    GameActor   m_actor;
    Board       m_board;

    // socket-related content
    ServerSocket::SPtr m_server_socket;

};

#endif // KATARENGA_CLIENT_GAME_HPP
