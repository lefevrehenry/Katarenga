#ifndef SERVER_HPP
#define SERVER_HPP

#include "server_utils.hpp"
#include <common/messages/message_utils.hpp>
#include <common/shared_utils.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <memory>
#include <string>
#include <thread>

namespace zmqpp {
    class context;
    class poller;
}

class Board;

/**
 * @brief The Player class lives in the main thread
 */
class Server
{

public:
    Server(ServerInfo& server_info);
    virtual ~Server();

public:
    void new_game(); // Init and start a new game
    void loop(); // Main loop during a game

private:
    /* Put here functions reacting to player messages */
    void process_player_check_connectivity(zmqpp::message& message);
    void process_player_move_message(zmqpp::message& message);
    void process_player_ask_board_configuration(zmqpp::message& message);
    void process_player_stop_game(zmqpp::message& message);

private:
    void stop_the_game();
    void process_command_line(const std::string& command);

private:
    void sendToBoth(zmqpp::message& message);
    void sendToPlayer(zmqpp::message& message, int player);
    void rejectMove(MoveMessage& move_msg);
    void sendWonMessage();
    void sendGameInit(int player);

private:
    // Socket-related content
    zmqpp::context  m_zmq_context;
    zmqpp::poller   m_poller;
    zmqpp::socket   m_white_player_socket;
    zmqpp::socket   m_black_player_socket;

    MessageReactor  m_player_reactor;

    // Game-related content
    std::unique_ptr<Board> m_board;
    int m_current_player;
    bool m_game_stopped;

    // Keep player connectivity state <white player, black player>
    std::pair<bool,bool> m_connectiviy;

};

#endif // SERVER_HPP

