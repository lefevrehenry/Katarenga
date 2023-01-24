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
    using ClientSockets = std::vector<ClientRegistry::ClientSocket::SPtr>;

public:
    Server(const ServerInfo& server_info);

public:
    void loop();

public:
    ClientRegistry* client_registry();

public:
    GameRegistry* game_registry();

public:
    zmqpp::endpoint_t create_new_client_endpoint() const;
    void start_monitor_client(ClientRegistry::ClientId id);
    void stop_monitor_client(ClientRegistry::ClientId id);

private:
    // Server-related content
    ServerInfo m_server_info;

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
