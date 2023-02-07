#include "game.hpp"

using Move = Common::Move;
using GameActor = Common::GameActor;

Game::Game() :
    m_status(Status::Pending),
    m_board(),
    m_white_socket(),
    m_black_socket()
{
    auto update_status = std::bind(&Game::update_status, this);

    white_player_joined.connect(std::bind(update_status));
    white_player_left.connect(std::bind(update_status));

    black_player_joined.connect(std::bind(update_status));
    black_player_left.connect(std::bind(update_status));

    std::string config = generateBoardCellTypes();
    m_board.setBoardCellTypes(config);
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

bool Game::has_white_player() const
{
    return bool(m_white_socket);
}

bool Game::has_black_player() const
{
    return bool(m_black_socket);
}

bool Game::is_white_player(const PlayerSocket::SPtr socket) const
{
    return m_white_socket == socket;
}

bool Game::is_black_player(const PlayerSocket::SPtr socket) const
{
    return m_black_socket == socket;
}

void Game::set_white_socket(const PlayerSocket::SPtr& socket)
{
    if(m_white_socket)
        m_white_socket->unmark_busy();

    m_white_socket = socket;

    if(m_white_socket)
        m_white_socket->mark_busy();

    if(socket)
        white_player_joined();
    else
        white_player_left();
}

void Game::set_black_socket(const PlayerSocket::SPtr& socket)
{
    if(m_black_socket)
        m_black_socket->unmark_busy();

    m_black_socket = socket;

    if(m_black_socket)
        m_black_socket->mark_busy();

    if(socket)
        black_player_joined();
    else
        black_player_left();

}

bool Game::play(const Move& move, GameActor player)
{
    if(m_board.getCurrentPlayer() != player)
        return false;

    return m_board.playMove(move);
}

void Game::update_status()
{
//    if(board->is_over())
//        m_status = Status::Finished;
//    else if
    if(has_white_player() && has_black_player())
        m_status = Status::Running;
    else
        m_status = Status::Pending;
}
