#ifndef KATARENGA_CLIENT_CLIENT_HPP
#define KATARENGA_CLIENT_CLIENT_HPP

// Katarenga
#include <common/common_utils.hpp>

#include <client/client_utils.hpp>
#include <client/game.hpp>
#include <client/sockets/connection_socket.hpp>
#include <client/sockets/server_socket.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// Nod
#include <nod/nod.hpp>

/**
 * @brief The Client class
 */
class Client
{
    using GameActor = Common::GameActor;

public:
    Client(const ServerInfo& server_info, const AppInfo& app_info);
    virtual ~Client();

public:
    zmqpp::context* context() const;
    zmqpp::endpoint_t app_endpoint() const;
    zmqpp::endpoint_t server_endpoint() const;

public:
    ServerSocket::SPtr server_socket() const;

public:
    void poll();
//    void quit();

public:
    void close_connection();
    void init_game(GameActor actor, const std::string& position, const ServerSocket::SPtr& socket);
    bool play_move(Common::Move move);

public:
    bool server_connected() const { return bool(m_server_socket); }
    bool in_game() const { return bool(m_game); }
    void request_position() const { if(in_game()) position_changed(m_game->position()); }

public:
    void open_server_socket(const zmqpp::endpoint_t& endpoint);
    void close_server_socket();

private:
    void create_game(GameActor actor);
    void destroy_game();

private:
    void process_command_line(const std::string& command);

public:
    nod::signal<void(const Common::Position& position)> position_changed;
    nod::signal<void(Common::Move)> move_played;

private:
    // Client-related content
    ServerInfo  m_server_info;
    AppInfo     m_app_info;
    bool        m_should_quit;

    // Socket-related content
    zmqpp::poller       m_poller;

    ConnectionSocket    m_connection_socket;
    ServerSocket::SPtr  m_server_socket;

    // Game-related content
    Game::SPtr m_game;

};

#endif // KATARENGA_CLIENT_CLIENT_HPP
