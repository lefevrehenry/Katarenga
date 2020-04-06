#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "utils.hpp"
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
class Player
{

public:
    Player(MainArguments &main_args);
    virtual ~Player();

public:
    void loop();    // function executed in the main thread

private:
    void process_server_board_configuration(zmqpp::message& message);
    void process_server_move_message(zmqpp::message& message);
    void process_server_player_won(zmqpp::message& message);
    void process_server_game_stopped(zmqpp::message& message);

private:
    // Socket-related content
    zmqpp::context  m_zmq_context;
    zmqpp::poller   m_poller;
    zmqpp::socket   m_server_thread_socket;
    zmqpp::socket   m_render_thread_socket;

    std::thread     m_render_thread;

    MessageReactor  m_server_thread_reactor;
    MessageReactor  m_render_thread_reactor;

    // Game-related content
    bool m_game_finished = false;
    int m_self_player;
    int m_current_player;
};

/* function executed in its own thread */
//void player_function();

#endif // PLAYER_HPP

