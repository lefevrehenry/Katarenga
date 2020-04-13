#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "utils.hpp"
#include <message/message_utils.hpp>

#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <string>
#include <thread>
#include <vector>

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
    Player(GameSettings& game_settings);
    virtual ~Player();

public:
    void loop();    // function executed in the main thread

private:
    void process_server_game_init(zmqpp::message& message);
    void process_server_board_configuration(zmqpp::message& message);
    void process_server_move_message(zmqpp::message& message);
    void process_server_player_won(zmqpp::message& message);
    void process_server_game_stopped(zmqpp::message& message);

    void process_graphics_case_clicked(zmqpp::message& message);
    void process_graphics_game_stopped(zmqpp::message& message);

private:
    void retrieve_piece_locations(const std::string& board_configuration);

    // Socket-related content
    zmqpp::context  m_zmq_context;
    zmqpp::poller   m_poller;
    zmqpp::socket   m_server_thread_socket;
    zmqpp::socket   m_render_thread_socket;

    std::thread     m_render_thread;

    MessageReactor  m_server_thread_reactor;
    MessageReactor  m_render_thread_reactor;

    // Game-related content
    std::vector<int> m_piece_locations; // The list of cell ids where my pieces are
    bool m_game_finished;
    int m_self_player;
    char m_self_player_sign;
    int m_current_player;
    bool m_game_stopped;
};

#endif // PLAYER_HPP

