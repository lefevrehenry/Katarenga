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
        msg_server("Unknow command '" + command + "'");
        return;
    }

    if(option_found == "h" || option_found == "help")
    {
        msg_server("h,help for help");
        msg_server("c,close to close the server");
    }
    else
    {
        msg_server("Unknow command '" + command + "'");
    }
}

Server::Server(const ServerInfo& server_info) :
    m_server_info(server_info),
    m_zmq_context(),
    m_poller(),
    m_connection_socket(this, &m_zmq_context, server_info.processus_endpoint),
    m_client_sockets(),
    m_client_registry(),
    m_game_registry()
{
    m_poller.add(m_connection_socket, zmqpp::poller::poll_in);
    m_poller.add(STDIN_FILENO, zmqpp::poller::poll_in);

    using R = std::function<void(const ClientRegistry::ClientId&)>;

    R start = std::bind(&Server::start_monitor_client, this, std::placeholders::_1);
    R stop = std::bind(&Server::stop_monitor_client, this, std::placeholders::_1);

    m_client_registry.client_added.connect(start);
    m_client_registry.client_removed.connect(stop);
}

void Server::loop()
{
    msg_server("Starting main loop of the server");

    while(true)
    {
        msg_server("poll");
        if(m_poller.poll(5000))//zmqpp::poller::wait_forever))
        {
            if(m_poller.has_input(m_connection_socket))
            {
                m_connection_socket.process_input_message();
            }

            for (const PlayerSocket::SPtr& socket : m_client_sockets) {
                if(m_poller.has_input(*socket))
                {
                    msg_server("message received");
                    socket->process_input_message();
                }
            }

            if(m_poller.has_input(STDIN_FILENO))
            {
                std::string command;
                std::getline(std::cin, command);

                // process the command read from std::cin
                // process_command_line(command);
            }
        } else {
            msg_server("break");
            break;
        }
    }

    msg_server("Exiting main loop of the server");
}

ClientRegistry* Server::client_registry()
{
    return &m_client_registry;
}

zmqpp::endpoint_t Server::create_new_client_endpoint() const
{
    static int N = 1;

    zmqpp::endpoint_t endpoint = m_server_info.processus_endpoint + std::to_string(N++);

    return endpoint;
}

void Server::start_monitor_client(ClientRegistry::ClientId id)
{
    ClientRegistry::ClientSocket socket = m_client_registry.socket(id);

    m_client_sockets.push_back(socket);
    m_poller.add(*socket.get(), zmqpp::poller::poll_in);
}

void Server::stop_monitor_client(ClientRegistry::ClientId id)
{
//    ClientRegistry::ClientSocket socket = m_client_registry.socket(id);

//    std::remove(m_open_sockets.begin(), m_open_sockets.end(), socket.get());
//    m_poller.remove(*socket.get());
}
