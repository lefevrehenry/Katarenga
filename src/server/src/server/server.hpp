#ifndef KATARENGA_SERVER_SERVER_HPP
#define KATARENGA_SERVER_SERVER_HPP

// Katarenga
#include <common/messages/messages.hpp>
#include <common/sockets/abstract_socket.hpp>

#include <server/client_registry.hpp>
#include <server/game_registry.hpp>
#include <server/server_utils.hpp>
#include <server/sockets/connection_socket.hpp>
#include <server/sockets/player_socket.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

class ConnectionSocket;

/**
 * @brief The Server class
 */
class Server
{
    using ClientSocket = ClientRegistry::ClientSocket;
    using ClientSockets = std::vector<ClientSocket::SPtr>;

public:
    Server(const ServerInfo& server_info);

public:
    zmqpp::context* context();
    zmqpp::endpoint_t endpoint() const;

public:
    void loop();

public:
    zmqpp::endpoint_t new_connection(const std::string& ip, const std::string& port);

private:
    void start_monitor_socket(const ClientSocket::SPtr& socket);
    void stop_monitor_socket(const ClientSocket::SPtr& socket);

private:
    void process_command_line(const std::string& command);

private:
    // Server-related content
    ServerInfo  m_server_info;
    bool        m_should_quit;

    // Socket-related content
    zmqpp::context      m_zmq_context;
    zmqpp::poller       m_poller;

    ConnectionSocket    m_connection_socket;
    ClientSockets       m_client_sockets;

    // Client-related content
    ClientRegistry m_client_registry;

    // Game-related content
    GameRegistry m_game_registry;

};

#endif // KATARENGA_SERVER_SERVER_HPP
