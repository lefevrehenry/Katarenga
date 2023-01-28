#include "game.hpp"

// Katarenga
#include <common/board/board_utils.hpp>

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

bool Game::has_server_socket() const
{
    return bool(m_server_socket);
}

void Game::set_server_socket(const ServerSocket::SPtr& socket)
{
    if(!socket)
        throw std::runtime_error("socket is null");

    m_server_socket = socket;

    if(socket)
        server_joined();
    else
        server_left();
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
