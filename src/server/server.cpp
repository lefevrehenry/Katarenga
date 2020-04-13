#include "server.hpp"
#include "Board.hpp"

Server::Server(ServerInfo &server_info) :
    m_zmq_context(),
    m_poller(),
    m_white_player_socket(m_zmq_context, zmqpp::socket_type::pair),
    m_black_player_socket(m_zmq_context, zmqpp::socket_type::pair),
    m_player_reactor()
{
    m_white_player_socket.bind(server_info.white_binding_point);
    m_black_player_socket.bind(server_info.black_binding_point);

    m_poller.add(m_white_player_socket, zmqpp::poller::poll_in);
    m_poller.add(m_black_player_socket, zmqpp::poller::poll_in);

    //using Callback = MessageReactor::Callback;
    /* Create the function callback here and add them to the player_reactor */
}

Server::~Server()
{
    m_white_player_socket.close();
    m_black_player_socket.close();
}

void Server::new_game()
{
    if (m_board != nullptr)
        delete m_board;

    m_board = new Board();
    m_board->setBoardCellTypes(generateBoardCellTypes());
    m_game_finished = false;
    m_current_player = 1;

    server_msg("New game created");
}

void Server::loop()
{
    server_msg("Starting main loop of the game");

    while(!m_game_finished)
    {
        zmqpp::message input_message;

        if(m_poller.poll(zmqpp::poller::wait_forever))
        {
            // First receive the message
            if(m_poller.has_input(m_white_player_socket))
            {
                m_white_player_socket.receive(input_message);
            }
            else if (m_poller.has_input(m_black_player_socket))
            {
                m_black_player_socket.receive(input_message);
            }
            else
            {
                server_msg("This should not happen, terminating");
                // TODO send a message to the players that something went wrong before terminating
                std::terminate();
            }

            // Then handle it
            m_player_reactor.process_message(input_message);
        }
    }
}
