#include "server.hpp"

// Katarenga
#include <common/board/Board.hpp>
#include <common/board/board_utils.hpp>

// Standard Library
#include <string>
#include <unistd.h>

Server::Server(const ServerInfo& server_info) :
    m_server_info(server_info),
    m_should_quit(false),
    m_zmq_context(),
    m_poller(),
    m_connection_socket(this, context(), endpoint()),
    m_client_sockets(),
    m_client_registry(),
    m_game_registry()
{
    m_poller.add(m_connection_socket, zmqpp::poller::poll_in);
    m_poller.add(STDIN_FILENO, zmqpp::poller::poll_in);

//    using R = std::function<void(const ClientRegistry::ClientId&)>;

//    R start = std::bind(&Server::start_monitor_client, this, std::placeholders::_1);
//    R stop = std::bind(&Server::stop_monitor_client, this, std::placeholders::_1);

//    m_client_registry.client_added.connect(start);
//    m_client_registry.client_removed.connect(stop);
}

zmqpp::context* Server::context()
{
    return &m_zmq_context;
}

zmqpp::endpoint_t Server::endpoint() const
{
    return m_server_info.processus_endpoint;
}

void Server::loop()
{
    msg_server("Starting main loop of the server");

    while(!m_should_quit)
    {
//        msg_server("poll");
        std::cout << ">>> " << std::flush;
        if(m_poller.poll(zmqpp::poller::wait_forever))
        {
            if(m_poller.has_input(m_connection_socket))
            {
                std::cout << std::endl;

//                msg_server("new connection request");
                m_connection_socket.process_input_message();
            }

            for (const PlayerSocket::SPtr& socket : m_client_sockets) {
                if(m_poller.has_input(*socket))
                {
                    std::cout << std::endl;

//                    msg_server("message received");
                    socket->process_input_message();
                }
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

    msg_server("Exiting main loop of the server");
}

static zmqpp::endpoint_t create_new_client_endpoint(const zmqpp::endpoint_t& endpoint)
{
    static int N = 1;

    zmqpp::endpoint_t client_endpoint = endpoint + std::to_string(N++);

    return client_endpoint;
}

zmqpp::endpoint_t Server::new_connection(const std::string& ip, const std::string& port)
{
    ClientRegistry::ClientId id = ClientRegistry::Id(ip, port);

    ClientRegistry* registry = &m_client_registry;

    if(registry->client_exists(id)) {
        ClientRegistry::ClientSocket::SPtr socket = registry->socket(id);

        return socket->endpoint();
    }

    ClientSocket::SPtr socket = std::make_shared<ClientSocket>(this, context(), create_new_client_endpoint(endpoint()));

    if(!registry->add_client(id, socket))
        return "";

    start_monitor_socket(socket);

    return socket->endpoint();
}

void Server::start_monitor_socket(const ClientSocket::SPtr& socket)
{
    m_client_sockets.push_back(socket);
    m_poller.add(*socket, zmqpp::poller::poll_in);
}

void Server::stop_monitor_socket(const ClientSocket::SPtr& socket)
{
    auto it = std::find(m_client_sockets.begin(), m_client_sockets.end(), socket);

    if(it == m_client_sockets.end())
        return;

    m_client_sockets.erase(it);
    m_poller.remove(*socket);
}

void Server::process_command_line(const std::string& command)
{
//    msg_server("running '" + command + "'");

    if(command.empty())
    {
        // does nothing
    }
    else if(command == "h" || command == "help")
    {
        msg_server("h,help for help");
        msg_server("s,status");
        msg_server("b,board");
        msg_server("q,quit for quit the server");
    }
    else if(command == "s" || command == "status")
    {
        msg_server(std::to_string(m_client_sockets.size()) + " clients connected");
    }
    else if(command == "b" || command == "board")
    {
        ServerCommon::GameId id = 1;
        Game::SPtr game = m_game_registry.m_games.count(id) ? m_game_registry.m_games.at(id) : nullptr;
        if(game) {
            game->print_board();
        } else {
            msg_server("No curent game");
        }

    }
    else if(command == "q" || command == "quit")
    {
        m_should_quit = true;
    }
    else
    {
        msg_server("unknow command '" + command + "'");
    }
}
