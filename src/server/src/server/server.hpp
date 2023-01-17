#ifndef KATARENGA_SERVER_SERVER_HPP
#define KATARENGA_SERVER_SERVER_HPP

// Katarenga
#include <common/messages/messages.hpp>

#include <server/client_registry.hpp>
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
    ClientRegistry& client_registry();

private:
    // Socket-related content
    zmqpp::context      m_zmq_context;
    zmqpp::poller       m_poller;

    ConnectionSocket    m_connection_socket;
    PlayerSockets       m_player_sockets;

    // Client-related content
    ClientRegistry m_client_registry;

    // Game-related content

};

#endif // KATARENGA_SERVER_SERVER_HPP

