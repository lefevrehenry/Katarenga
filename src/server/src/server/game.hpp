#ifndef KATARENGA_SERVER_GAME_HPP
#define KATARENGA_SERVER_GAME_HPP

// Katarenga
#include <common/board/Board.hpp>
#include <common/common_utils.hpp>
#include <server/server_utils.hpp>
#include <server/sockets/player_socket.hpp>

// Nod
#include <nod/nod.hpp>

// Standard Library
#include <memory>

/**
 * @brief The Game class (server side)
 */
class Game
{
public:
    using SPtr = std::shared_ptr<Game>;

    enum class Status {
        Pending,        // waiting for players
        Running,        // players are playing
        Finished        // game is over
    };

public:
    Game();

public:
    static ServerCommon::GameId GenerateId();
    static Game::SPtr Create();

public:
    bool is_pending() { return m_status == Status::Pending; }
    bool is_running() { return m_status == Status::Running; }
    bool is_over() { return m_status == Status::Finished; }

    bool has_white_player() const;
    bool has_black_player() const;

    bool is_white_player(const PlayerSocket::SPtr& socket) const;
    bool is_black_player(const PlayerSocket::SPtr& socket) const;

    Common::GameActor actor(const PlayerSocket::SPtr& socket) const;

public:
    void set_white_socket(const PlayerSocket::SPtr& socket);
    void set_black_socket(const PlayerSocket::SPtr& socket);

public:
    bool play(const Common::Move& move, Common::GameActor player);
    void print_board() const
    {
        Common::Position configuration = m_board.getBoardConfiguration();

        std::cout << configuration << std::endl;
    }

public:
    Common::Position position() const { return m_board.getBoardConfiguration(); }

public:
    nod::signal<void()> white_player_joined;
    nod::signal<void()> white_player_left;

    nod::signal<void()> black_player_joined;
    nod::signal<void()> black_player_left;

private:
    void update_status();

private:
    // board-related content
    Status m_status;
    Board  m_board;

    // socket-related content
    PlayerSocket::SPtr m_white_socket;
    PlayerSocket::SPtr m_black_socket;

};

#endif // KATARENGA_SERVER_GAME_HPP
