#ifndef KATARENGA_CLIENT_CLIENT_HPP
#define KATARENGA_CLIENT_CLIENT_HPP

// Katarenga
#include <common/common_utils.hpp>

#include <client/game.hpp>
#include <client/sockets/connection_socket.hpp>
#include <client/sockets/server_socket.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

/**
 * @brief The Client class
 */
class Client
{
    using GameActor = Common::GameActor;

public:
    Client(const ServerInfo& server_info);
    virtual ~Client();

public:
    ConnectionSocket::SPtr connection_socket() const;
    ServerSocket::SPtr server_socket() const;

public:
    int exec();

public:
    Game::SPtr game() const;

    bool server_connected() const { return bool(m_server_socket); }
    bool in_game() const { return bool(m_game); }

public:
    void open_server_socket(const zmqpp::endpoint_t& endpoint);
    void close_server_socket();

public:
    void create_game(GameActor actor);
    void destroy_game();

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
    ServerSocket::SPtr  m_server_socket;

    // Game-related content
    Game::SPtr m_game;

};

#endif // KATARENGA_CLIENT_CLIENT_HPP