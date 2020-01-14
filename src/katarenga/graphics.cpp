#include "network_utils.hpp"
#include "graphics.hpp"
#include "../server/Board.hpp"

#include <iostream>

using std::cout;        // At some point
using std::cin;         // We'll need to
using std::endl;        // Get rid of
using std::string;      // These
using std::to_string;   // Declarations


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

void graphics_function(int this_player, int socket_port, bool verbose)
{
    string socket_endpoint = "tcp://127.0.0.1:" + to_string(socket_port);
    zmqpp::context_t context;
    zmqpp::socket_t socket(context, zmqpp::socket_type::pair);
    socket.connect(socket_endpoint);

    string board_configuration = s_recv(socket);
    s_send(socket, "ACK");

    cout << "GL received board config:\n" << board_configuration << endl;

//    Board board;

    cout << "GL thread ready to play!" << endl;

    // Main loop
    string move_str;
    bool end_game = false;
    int has_won = 0;
    while(!end_game)
    {
//        if (board.getCurrentPlayer() == this_player)
//        {
//            // It's our turn
//            cout << "Enter your string move of the form \"m<src_cell_index>:<dest_cell_index>\" ";
//            cin >> move_str;

//            s_send(socket, move_str);
//            /*if(!polling(socket, 5))
//            {
//                // TODO throw an error
//                cout << "ERROR WHILE POLLING, terminating GL thread." << endl;
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
//                cout << "The move was not valid" << endl;
//            }
//            else
//            {
//                cout << "ERROR: Weird message received from player thread, terminating." << endl;
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
//                cout << "ERROR WHILE POLLING, terminating GL thread." << endl;
//                socket.close();
//                return;
//            }*/
//            move_str = s_recv(socket);
//            board.playMove(move_str);
//            cout << "Other player just played " << move_str << endl;
//        }

//        if ((has_won = board.gameFinished()) != 0)
//        {
//            end_game = true;
//            cout << "Woah! " << (has_won == 1?"White":"Black") << " player has won the game!"
//                                                                  "" << endl;
//        }
    }


    socket.close();
    cout << "Terminating GL thread." << endl;
}



void standalone_graphics_function(int socket_port, bool verbose)
{
    string socket_endpoint = "tcp://127.0.0.1:" + to_string(socket_port);
    zmqpp::context_t context;
    zmqpp::socket_t socketS(context, zmqpp::socket_type::pair);
    socketS.connect(socket_endpoint);

    string board_configuration = s_recv(socketS);
    s_send(socketS, "ACK");

    cout << "GL received board config:\n" << board_configuration << endl;

//    Board board;

    cout << "GL thread ready to play!" << endl;

    // Main loop
    string move_str;
    string ret;
    bool end_game = false;
    int has_won = 0;
    int current_player;

//    board.print();
    while(!end_game)
    {
//        ret = s_recv(socketS);
//        if (ret[0] == 'p')
//        {
//            // It's a player turn
//            current_player = (ret[1] == '1' ? 1 : -1);

//            move_str = askNextMoveText(current_player);

//            s_send(socketS, move_str);
//            ret = s_recv(socketS);

//            if (ret[0] == 'm')
//            {
//                // The move has been accepted, apply it
//                board.playMove(ret);
//                board.print();
//            }
//        }
//        else if (ret[0] == 'w')
//        {
//            // A player has won
//            has_won = (ret[1] == '1' ? 1 : -1);

//            // Just stop the loop and terminate
//            end_game = true;
//        }
    }

    socketS.close();
    cout << "Terminating GL thread." << endl;
}


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
