#include "graphics.hpp"
#include "player.hpp"
#include "utils.hpp"

#include <GLTK/GLFWApplication.h>

#include <iostream>

using MessageType = MessageWrapper::MessageType;

void print_help()
{
    render_msg("h,help for help");
    render_msg("c,click for click in a case");
    render_msg("p,print to print the board");
    render_msg("s,stop for quit");
}

Graphics::Graphics(zmqpp::context& zmq_context, const std::string& main_thread_binding_point) :
    m_poller(),
    m_main_thread_socket(zmq_context, zmqpp::socket_type::pair),
    m_main_thread_reactor(),
    m_end_game(false)
{
    m_main_thread_socket.connect(main_thread_binding_point);
}

Graphics::~Graphics()
{
    m_main_thread_socket.close();
}

void Graphics::loop()
{
    render_msg("GL thread ready to play!");

    print_help();

    while(!m_end_game)
    {
        render_msg("Enter a command: ");

        std::string command = "";
        std::cin >> command;

        if(command == "h" || command == "help")
        {
            print_help();
        }
        else if(command == "c" || command == "click")
        {
            render_msg("Enter your string as the index of the cell '<src_cell_index>' ");

            std::string move_str;
            std::cin >> move_str;

            zmqpp::message message = ConstructMessage<CaseClicked>(move_str);

            // envoie le message (non bloquant)
            bool ret = m_main_thread_socket.send(message, true);

            if(!ret)
                render_msg("(click) error, message not sent");
        }
        else if(command == "p" || command == "print")
        {
            zmqpp::message message = ConstructMessage<AskBoardConfiguration>();

            // envoie le coup (non bloquant)
            bool ret = m_main_thread_socket.send(message, true);

            if(!ret)
                render_msg("(print) error, message not sent");
        }
        else if(command == "s" || command == "stop")
        {
            zmqpp::message message = ConstructMessage<StopGame>("human decide to stop", 0);

            // envoie le message (non bloquant)
            bool ret = m_main_thread_socket.send(message, true);

            if(!ret)
                render_msg("(stop) error, message not sent");
            else
                m_end_game = true;
        }
        else
        {
            render_msg("unknow command '" + command + "'");
        }

//        if ((has_won = board.gameFinished()) != 0)
//        {
//            end_game = true;
//            render_msg("Woah! " << (has_won == 1?"White":"Black") << " player has won the game!"
//                                                                  "");
//        }
    }

    render_msg("Terminating");
}

void graphics_function(zmqpp::context& zmq_context, const std::string& main_thread_binding_point)
{
    Graphics graphic(zmq_context, main_thread_binding_point);

    graphic.loop();
}
