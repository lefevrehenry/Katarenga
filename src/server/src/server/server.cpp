#include "server.hpp"

// Katarenga
#include <common/board/Board.hpp>
#include <common/board/board_utils.hpp>

// Standard Library
#include <string>
#include <unistd.h>

static void process_command_line(const std::string& command)
{
    if(command.size() == 0)
        return;

    // all available options here
    static std::list<std::string> options = {"help", "h", "print", "p", "move", "m", "close", "c"};

    std::string option_found = "";

    // try to find the option matching the command
    for (const std::string& option : options)
    {
        size_t option_size = option.size();

        if(command.size() >= option_size && command.substr(0, option_size) == option) {
            option_found = option;
            break;
        }
    }

    if(option_found.size() == 0) {
        server_msg("Unknow command '" + command + "'");
        return;
    }

    if(option_found == "h" || option_found == "help")
    {
        server_msg("h,help for help");
        server_msg("c,close to close the server");
    }
    else
    {
        server_msg("Unknow command '" + command + "'");
    }
}

Server::Server(const ServerInfo& server_info) :
    m_zmq_context(),
    m_poller(),
    m_connection_socket(this, &m_zmq_context, server_info.processus_endpoint)
{
    m_poller.add(m_connection_socket, zmqpp::poller::poll_in);
    m_poller.add(STDIN_FILENO, zmqpp::poller::poll_in);
}

void Server::loop()
{
    server_msg("Starting main loop of the server");

    while(true)
    {
        if(m_poller.poll(5000))//zmqpp::poller::wait_forever))
        {
            if(m_poller.has_input(m_connection_socket))
            {
                server_msg("message received");
                m_connection_socket.process();
            }
            else if(m_poller.has_input(STDIN_FILENO))
            {
                std::string command;
                std::getline(std::cin, command);

                // process the command read from std::cin
                // process_command_line(command);
            }
        } else {
            break;
        }
    }

    server_msg("Exiting main loop of the server");
}
