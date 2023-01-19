#ifndef KATARENGA_SERVER_SERVER_HPP
#define KATARENGA_SERVER_SERVER_HPP

// Katarenga
#include <common/messages/messages.hpp>

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
    using PlayerSockets = std::map<int, std::unique_ptr<PlayerSocket>>;

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
    void client_added(ClientRegistry::ClientId id);
    void client_removed(ClientRegistry::ClientId id);

private:
    // Server-related content
    ServerInfo m_server_info;

    // Socket-related content
    zmqpp::context      m_zmq_context;
    zmqpp::poller       m_poller;

    ConnectionSocket    m_connection_socket;
    PlayerSockets       m_player_sockets;

    // Client-related content
    ClientRegistry m_client_registry;

    // Game-related content
    GameRegistry m_game_registry;

};

#endif // KATARENGA_SERVER_SERVER_HPP

