#include "graphics.hpp"
#include "player.hpp"
#include "utils.hpp"
#include <message/message_utils.hpp>

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

void graphics_function(zmqpp::context& zmq_context, const std::string& render_binding_point)
{
    zmqpp::socket socket_main_thread(zmq_context, zmqpp::socket_type::pair);
    socket_main_thread.connect(render_binding_point);

    render_msg("GL thread ready to play!");

    // Main loop
    bool end_game = false;
//    int has_won = 0;

    print_help();

    while(!end_game)
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
            bool ret = socket_main_thread.send(message, true);

            if(!ret)
                render_msg("(click) error, message not sent");
        }
        else if(command == "p" || command == "print")
        {
            zmqpp::message message = ConstructMessage<AskBoardConfiguration>();

            // envoie le coup (non bloquant)
            bool ret = socket_main_thread.send(message, true);

            if(!ret)
                render_msg("(print) error, message not sent");
        }
        else if(command == "s" || command == "stop")
        {
            zmqpp::message message = ConstructMessage<StopGame>("human decide to stop", 0);

            // envoie le message (non bloquant)
            bool ret = socket_main_thread.send(message, true);

            if(!ret)
                render_msg("(stop) error, message not sent");
            else
                end_game = true;
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

    socket_main_thread.close();

    render_msg("Terminating");
}



//void standalone_graphics_function(int socket_port, bool verbose)
//{
//    string socket_endpoint = "tcp://127.0.0.1:" + to_string(socket_port);
//    zmqpp::context_t context;
//    zmqpp::socket_t socketS(context, zmqpp::socket_type::pair);
//    socketS.connect(socket_endpoint);

//    string board_configuration = s_recv(socketS);
//    s_send(socketS, "ACK");

//    render_msg("GL received board config:\n" << board_configuration);

////    Board board;

//    render_msg("GL thread ready to play!");

//    // Main loop
//    string move_str;
//    string ret;
//    bool end_game = false;
//    int has_won = 0;
//    int current_player;

////    board.print();
//    while(!end_game)
//    {
////        ret = s_recv(socketS);
////        if (ret[0] == 'p')
////        {
////            // It's a player turn
////            current_player = (ret[1] == '1' ? 1 : -1);

////            move_str = askNextMoveText(current_player);

////            s_send(socketS, move_str);
////            ret = s_recv(socketS);

////            if (ret[0] == 'm')
////            {
////                // The move has been accepted, apply it
////                board.playMove(ret);
////                board.print();
////            }
////        }
////        else if (ret[0] == 'w')
////        {
////            // A player has won
////            has_won = (ret[1] == '1' ? 1 : -1);

////            // Just stop the loop and terminate
////            end_game = true;
////        }
//    }

//    socketS.close();
//    render_msg("Terminating GL thread.");
//}
