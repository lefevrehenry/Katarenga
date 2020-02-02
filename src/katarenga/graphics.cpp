#include "graphics.hpp"
#include "player.hpp"

#include <zmqpp/zmqpp.hpp>

#include <iostream>

#include <GLTK/GLFWApplication.h>

//using std::std::cout;        // At some point
//using std::cin;         // We'll need to
//using std::std::endl;        // Get rid of
//using std::string;      // These
//using std::to_string;   // Declarations


/*bool polling(socket_t & socket, int timeout)
{
    zmqpp::pollitem_t items [] = {{ socket, 0, ZMQ_POLLIN, 0}};
    int n = 0;
    while (1)
    {
        zmqpp::poll(&items[0], 1, timeout);
        if (items[0].revents & ZMQ_POLLIN)
        {
            return true;
        }

        // just a little guard for debugging
        if (++n == 6)
        {
            return false;
        }
    }
}*/

void msg1(const std::string& msg)
{
    std::cout << "render thread(" << msg << ")" << std::endl;
}

void graphics_function()
{
    zmqpp::context* context = PlayerInfo.context;
    std::string render_binding_point = PlayerInfo.render_binding_point;

    zmqpp::socket socket_main_thread(*context, zmqpp::socket_type::pair);
    socket_main_thread.connect("inproc://" + render_binding_point);

//    std::string board_configuration = s_recv(socket);
//    s_send(socket, "ACK");

//    std::cout << "GL received board config:\n" << board_configuration << std::endl;

//    Board board;

    std::cout << "GL thread ready to play!" << std::endl;

    // Main loop
    std::string move_str;
    bool end_game = false;
//    int has_won = 0;

    while(!end_game)
    {
//        if (board.getCurrentPlayer() == this_player)
//        {
//            // It's our turn
            std::cout << "Enter your string move of the form \"m<src_cell_index>:<dest_cell_index>\" ";
            std::cin >> move_str;

            zmqpp::message message;
            message << move_str;

            // envoie le coup
            socket_main_thread.send(message);

            if(move_str == "stop") {
                end_game = true;
                continue;
            }

//            s_send(socket, move_str);
//            /*if(!polling(socket, 5))
//            {
//                // TODO throw an error
//                std::cout << "ERROR WHILE POLLING, terminating GL thread." << std::endl;
//                socket.close();
//                return;
//            }*/
//            string ret = s_recv(socket);
//            if (ret == "accept")
//            {
//                board.playMove(move_str);
//            }
//            else if (ret == "reject")
//            {
//                std::cout << "The move was not valid" << std::endl;
//            }
//            else
//            {
//                std::cout << "ERROR: Weird message received from player thread, terminating." << std::endl;
//                socket.close();
//                return;
//            }
//        }
//        else
//        {
//            // It's the opponent turn
//            /*if(!polling(socket, 5))
//            {
//                // TODO throw an error
//                std::cout << "ERROR WHILE POLLING, terminating GL thread." << std::endl;
//                socket.close();
//                return;
//            }*/
//            move_str = s_recv(socket);
//            board.playMove(move_str);
//            std::cout << "Other player just played " << move_str << std::endl;
//        }

//        if ((has_won = board.gameFinished()) != 0)
//        {
//            end_game = true;
//            std::cout << "Woah! " << (has_won == 1?"White":"Black") << " player has won the game!"
//                                                                  "" << std::endl;
//        }
    }

    socket_main_thread.close();

    std::cout << "Terminating render thread." << std::endl;
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
