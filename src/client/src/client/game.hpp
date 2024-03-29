#ifndef KATARENGA_CLIENT_GAME_HPP
#define KATARENGA_CLIENT_GAME_HPP

// Katarenga
#include <common/board/Board.hpp>
#include <common/common_utils.hpp>

#include <client/sockets/server_socket.hpp>

// Nod
#include <nod/nod.hpp>

// Standard Library
#include <memory>

/**
 * @brief The Game class (client side)
 */
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

public:
    GameActor actor() const;
    Common::Position position() const;

public:
    bool has_server_socket() const;
    void set_server_socket(const ServerSocket::SPtr& socket);

public:
    void init_from_position(const std::string& position, GameActor actor);

public:
    bool request_play(const Common::Move& move);
    bool play(const Common::Move& move);
    void print_board() const;

public:
    nod::signal<void()> server_joined;
    nod::signal<void()> server_left;
    nod::signal<void(Common::Move)> move_played;

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
