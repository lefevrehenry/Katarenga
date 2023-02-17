#ifndef KATARENGA_SERVER_SERVER_HPP
#define KATARENGA_SERVER_SERVER_HPP

// Katarenga
#include <common/common_utils.hpp>
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
    using ClientId = ClientRegistry::ClientId;
    using ClientSocket = ClientRegistry::ClientSocket;

    using ClientSockets = std::vector<ClientSocket::SPtr>;

    using GameId = Common::GameId;
    using GameActor = Common::GameActor;
    using Move = Common::Move;

public:
    Server(const ServerInfo& server_info);

private:
    zmqpp::context* context();
    zmqpp::endpoint_t endpoint() const;

public:
    void loop();

public:
    zmqpp::endpoint_t new_connection(const std::string& ip, const std::string& port);
    void close_connection(const ClientSocket::SPtr& socket);

    GameId create_game(GameActor actor, const ClientSocket::SPtr& socket);
    void join_game(GameId id, const ClientSocket::SPtr& socket);
    void spectate_game(GameId id, const ClientSocket::SPtr& socket);

    std::string game_position(GameId id) const;

    bool play_move(GameId id, Move move, GameActor actor);

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
