#ifndef SERVER_HPP
#define SERVER_HPP

#include "server_utils.hpp"
#include <message/message_utils.hpp>

#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <string>
#include <thread>

namespace zmqpp {
    class context;
    class poller;
}

/**
 * @brief The Player class lives in the main thread
 */
class Server
{

public:
    Server(ServerInfo &server_info);
    virtual ~Server();

public:
    void new_game(); // Init and start a new game
    void loop(); // Main loop during a game

private:
    /* Put here functions reacting to player messages */

private:
    // Socket-related content
    zmqpp::context  m_zmq_context;
    zmqpp::poller   m_poller;
    zmqpp::socket   m_white_player_socket;
    zmqpp::socket   m_black_player_socket;

    MessageReactor  m_player_reactor;

    // Game-related content
    Board *m_board = nullptr;
    bool m_game_finished;
    int m_current_player;
};

#endif // SERVER_HPP

