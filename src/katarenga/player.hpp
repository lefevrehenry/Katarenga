#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <message/message_utils.hpp>

#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <string>
#include <thread>

namespace zmqpp {
    class context;
    class poller;
}

//struct PlayerInfo {
//    zmqpp::context* zmq_context;
//    int self_player; // 1 for White, -1 for Black
//    int current_player; // Which turn is it
//    bool game_finished;
//    const std::string render_binding_point = "inproc://katarenga-render-thread";

//};

//extern PlayerInfo PlayerInfo;

/**
 * @brief The Player class live in the main thread
 */
class Player
{

public:
    Player();
    virtual ~Player();

public:
    void loop();    // function executed in the main thread

private:
    void process_server_board_configuration(zmqpp::message& message);
    void process_server_move_message(zmqpp::message& message);
    void process_server_player_won(zmqpp::message& message);
    void process_server_game_stopped(zmqpp::message& message);

private:
    zmqpp::context  m_zmq_context;
    zmqpp::poller   m_poller;

    zmqpp::socket   m_server_thread_socket;
    zmqpp::socket   m_render_thread_socket;

    std::thread     m_render_thread;

    MessageReactor  m_server_thread_reactor;
    MessageReactor  m_render_thread_reactor;

};

/* function executed in its own thread */
//void player_function();

#endif // PLAYER_HPP

