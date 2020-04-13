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
    void process_player_move_message(zmqpp::message& message);
    void process_player_ask_board_configuration(zmqpp::message& message);
    void process_player_stop_game(zmqpp::message& message);

private:
    void sendToBoth(zmqpp::message& message);
    void sendToPlayer(zmqpp::message& message, int player);
    void rejectMove(MoveMessage& move_msg);
    void sendWonMessage();

    // Socket-related content
    zmqpp::context  m_zmq_context;
    zmqpp::poller   m_poller;
    zmqpp::socket   m_white_player_socket;
    zmqpp::socket   m_black_player_socket;

    MessageReactor  m_player_reactor;

    // Game-related content
    Board *m_board = nullptr;
    int m_current_player;
    bool m_game_stopped;
};

#endif // SERVER_HPP

