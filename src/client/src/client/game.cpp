#include "game.hpp"

// Katarenga
#include <common/board/board_utils.hpp>
#include <common/messages/messages.hpp>
#include <client/client_utils.hpp>

Game::Game(GameActor actor) :
    m_status(Status::Pending),
    m_actor(actor),
    m_board(),
    m_server_socket()
{
    auto update_status = std::bind(&Game::update_status, this);

    server_joined.connect(std::bind(update_status));
    server_left.connect(std::bind(update_status));

    std::string config = generateBoardCellTypes();
    m_board.setBoardCellTypes(config);
}

Game::GameActor Game::actor() const
{
    return m_actor;
}

Common::Position Game::position() const
{
    return m_board.getBoardConfiguration();
}

bool Game::has_server_socket() const
{
    return bool(m_server_socket);
}

void Game::set_server_socket(const ServerSocket::SPtr& socket)
{
//    if(m_server_socket)
//        m_server_socket->unmark_busy();

    m_server_socket = socket;

//    if(m_server_socket)
//        m_server_socket->mark_busy();

    if(socket)
        server_joined();
    else
        server_left();
}

void Game::init_from_position(const std::string& position, GameActor actor)
{
    m_actor = actor;
    m_board.setFromBoardConfiguration(position);
}

bool Game::request_play(const Common::Move& move)
{
    if(!is_running())
        return false;

    if(m_board.getCurrentPlayer() != m_actor)
        return false;

    if(!m_board.isValidMove(move, m_actor))
        return false;

    typename PlayMove::Request::Parameters p;
    p.move = move;

    // by-pass the engine
    m_server_socket->send_message<PlayMove::Request>(p);

    return true;
}

bool Game::play(const Common::Move& move)
{
    if(!is_running())
        return false;

    if(m_board.getCurrentPlayer() != m_actor)
        return false;

    bool ok = m_board.playMove(move);

    if(ok)
        move_played(move);

    return ok;
}

void Game::print_board() const
{
    std::string configuration = m_board.getBoardConfiguration();

    std::cout << configuration << std::endl;
}

void Game::update_status()
{
//    if(board->is_over())
//        m_status = Status::Finished;
//    else if
    if(has_server_socket())
        m_status = Status::Running;
    else
        m_status = Status::Pending;
}
