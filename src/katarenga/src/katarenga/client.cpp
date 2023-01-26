#include "client.hpp"

// Katarenga
#include <common/messages/message.hpp>
#include <common/messages/messages.hpp>
#include <katarenga/client_utils.hpp>

// Standard Library
#include <string>
#include <unistd.h>

Client::Client(const ServerInfo& server_info) :
    m_server_info(server_info),
    m_should_quit(false),
    m_zmq_context(),
    m_poller(),
    m_connection_socket(this, &m_zmq_context, server_info.processus_endpoint),
    m_game(nullptr)
{
    m_poller.add(m_connection_socket, zmqpp::poller::poll_in);
    m_poller.add(STDIN_FILENO, zmqpp::poller::poll_in);
}

int Client::exec()
{
    msg_client("Starting main loop of the client");

    msg_client("sending connection request ...");

//    {
//        m_connection_socket.request<NewConnection>();
//    }

    while(!m_should_quit)
    {
//        msg_client("poll");
        std::cout << ">>> " << std::flush;
        if(m_poller.poll(zmqpp::poller::wait_forever))
        {
            if(m_poller.has_input(m_connection_socket))
            {
                m_connection_socket.process_input_message();
            }

            if(m_server_socket && m_poller.has_input(*m_server_socket.get()))
            {
                m_server_socket->process_input_message();
            }

            if(m_poller.has_input(STDIN_FILENO))
            {
                std::string command;
                std::getline(std::cin, command);

                // process the command read from std::cin
                process_command_line(command);
            }
        }
    }

    msg_client("client ending");

    return 0;
}

ConnectionSocket::SPtr Client::connection_socket() const
{
    return nullptr;
}

ServerSocket::SPtr Client::server_socket() const
{
    return m_server_socket;
}

Game::SPtr Client::game() const
{
    return m_game;
}

void Client::open_server_socket(const zmqpp::endpoint_t& endpoint)
{
    if(m_server_socket)
        return;

    m_server_socket = std::make_shared<ServerSocket>(this, &m_zmq_context, endpoint);

    m_poller.add(*m_server_socket.get(), zmqpp::poller::poll_in);
}

void Client::close_server_socket()
{
    if(!m_server_socket)
        return;

    m_poller.remove(*m_server_socket.get());

    m_server_socket.reset();
}

Game::SPtr Client::create_game()
{
    m_game = std::make_shared<Game>();

    return m_game;
}

void Client::destroy_game()
{
    m_game.reset();
}

void Client::process_command_line(const std::string& command)
{
//    msg_client("running '" + command + "'");

    if(command == "h" || command == "help")
    {
        msg_client("h,help for help");
//        msg_client("c,click for click in a case");
        msg_client("p,print to print the board");
        msg_client("co,connect for start a connection with the server");
        msg_client("disco,disconnect for disconnect with the server");
        msg_client("q,quit for quit the Katarenga");
    }
//    else if(command == "c" || command == "click")
//    {
//        std::cout << "Enter your string as the index of the cell '<src_cell_index>' ";

//        std::string move_str;
//        std::cin >> move_str;
//    }
    else if(command == "p" || command == "print")
    {
        if(!m_game) {
            msg_client("No current game");
        } else {
            // TODO: print the board
            msg_client("incoming ...");
        }
    }
    else if(command == "co" || command == "connect")
    {
        if(m_server_socket) {
            msg_client("nothing done");
        } else {
            m_connection_socket.request<NewConnection>();
        }
    }
    else if(command == "disco" || command == "disconnect")
    {
        if(!m_server_socket) {
            msg_client("nothing done");
        } else {
            m_server_socket->send_message<CloseConnection>();
        }
    }
    else if(command == "q" || command == "quit")
    {
        m_should_quit = true;
    }
    else
    {
        msg_client("unknow command '" + command + "'");
    }
}
