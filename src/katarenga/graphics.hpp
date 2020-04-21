#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <message/message_utils.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <string>

class Graphics
{

public:
    Graphics(zmqpp::context& zmq_context, const std::string& main_thread_binding_point);
    virtual ~Graphics();

public:
    void loop();

private:
    void process_main_answer_board_configuration(zmqpp::message& message);
    void process_main_answer_move_message(zmqpp::message& message);

private:
    zmqpp::poller   m_poller;
    zmqpp::socket   m_main_thread_socket;

    MessageReactor  m_main_thread_reactor;

    bool m_end_game;

};

void graphics_function(zmqpp::context& zmq_context, const std::string& main_thread_binding_point);

#endif // GRAPHICS_HPP
