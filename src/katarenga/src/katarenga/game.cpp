#include "game.hpp"

// Katarenga
#include <common/board/board_utils.hpp>
#include <common/messages/messages.hpp>
#include <katarenga/client_utils.hpp>

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

void Game::print_board() const
{
    std::string configuration = m_board.getBoardConfiguration();

    std::cout << configuration << std::endl;
}

void Game::play(const Common::Move& move)
{
    if(!is_running()) {
        msg_client("Cannot play");
        return;
    }

    typename PlayMove::Parameters p;
    p.move = move;

    m_server_socket->send_message<PlayMove>(p);
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
