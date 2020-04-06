#include "graphics.hpp"
#include "player.hpp"
#include "utils.hpp"
#include <message/message_utils.hpp>

#include <GLTK/GLFWApplication.h>

using MessageType = MessageWrapper::MessageType;


void graphics_function(zmqpp::context &zmq_context, std::string render_binding_point)
{
    zmqpp::socket socket_main_thread(zmq_context, zmqpp::socket_type::pair);
    socket_main_thread.connect(render_binding_point);

    render_msg("GL thread ready to play!");

    // Main loop
    bool end_game = false;
//    int has_won = 0;

    while(!end_game)
    {
        std::cout << "Enter a command: " << std::endl;

        std::string command = "";
        std::cin >> command;

        if(command == "h" || command == "help")
        {
            std::cout << "h,help for help" << std::endl;
            std::cout << "c,click for click in a case" << std::endl;
            std::cout << "p,print to print the board" << std::endl;
            std::cout << "s,stop for quit" << std::endl;
        }
        else if(command == "c" || command == "click")
        {
            std::cout << "Enter your string as the index of the cell '<src_cell_index>' ";

            std::string move_str;
            std::cin >> move_str;
        }
        else if(command == "p" || command == "print")
        {
            zmqpp::message message;

            MessageType type = MessageType::AskBoardConfiguration;
            message.add(&type, sizeof(MessageType));

            // envoie le coup (non bloquant)
            bool ret = socket_main_thread.send(message, true);

            if(!ret)
                std::cout << "(print) error, message not sent" << std::endl;
        }
        else if(command == "s" || command == "stop")
        {
            zmqpp::message message;

            MessageType type = MessageType::StopGame;
            message.add(&type, sizeof(MessageType));

            // envoie le message (non bloquant)
            bool ret = socket_main_thread.send(message, true);

            if(!ret)
                std::cout << "(stop) error, message not sent" << std::endl;
            else
                end_game = true;
        }
        else
        {
            std::cout << "unknow command '" << command << "'" << std::endl;
        }

//        if ((has_won = board.gameFinished()) != 0)
//        {
//            end_game = true;
//            std::cout << "Woah! " << (has_won == 1?"White":"Black") << " player has won the game!"
//                                                                  "" << std::endl;
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

//    std::cout << "GL received board config:\n" << board_configuration << std::endl;

////    Board board;

//    std::cout << "GL thread ready to play!" << std::endl;

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
//    std::cout << "Terminating GL thread." << std::endl;
//}


std::string askNextMoveText(int current_player)
{
    int srci, desti;

    std::cout << (current_player == 1 ? "It's White's (+)" : "Black's (-)") << " turn, what is your move?" << std::endl;
    std::cout << "Source index: ";
    std::cin >> srci;
    std::cout << "Destination index: ";
    std::cin >> desti;


    std::string src_str = (srci < 10 ? "0" : "") + std::to_string(srci);
    std::string dest_str = (desti < 10 ? "0" : "") + std::to_string(desti);

    return "m" + src_str + ":" + dest_str;
}
