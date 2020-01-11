#include "network_utils.hpp"
#include "server.hpp"
#include "Board.hpp"

#include <iostream>

using std::cout;        // At some point we'll
using std::endl;        // Need to get rid of
using std::string;      // These
using std::to_string;   // Declarations

void server_function(const int socket_port, const bool verbose)
{
//    // Open sockets for White and Black players
//    zmqpp::context context;
//    zmqpp::socket socket(context, zmqpp::socket_type::pull);
//    socket.bind("tcp://*:" + to_string(socket_port));

//    // Setup game
//    string board_configuration = "";
//    Board board(board_configuration, verbose);

//    // Send the board configuration to players and recv ACKs
//    if(verbose){ cout << "Sending board configuration to players" << endl; }
////    s_send(socket, board_configuration);
////    s_send(socketB, board_configuration);
////    s_recv(socket);
////    s_recv(socketB);

//    // Main loop
//    string move_str;
//    bool end_game = false;
//    int has_won = 0;
//    while (!end_game)
//    {
//        board.print();

//        // TODO put all this in a generic function depending on the current player?
//        if(board.getCurrentPlayer() == 1)
//        {
//            // Waiting for White to send his move
//            move_str = s_recv(socket);
//            if(!board.isValidMove(move_str, 1))
//            {
//                s_send(socket, "reject");
//            }
//            else
//            {
//                board.playMove(move_str);
//                s_send(socket, move_str);
//                s_send(socketB, move_str);
//            }
//        }
//        else
//        {
//            move_str = s_recv(socketB);
//            if(!board.isValidMove(move_str, -1))
//            {
//                s_send(socketB, "reject");
//            }
//            else
//            {
//                board.playMove(move_str);
//                s_send(socket, move_str);
//                s_send(socketB, move_str);
//            }
//        }

//        cout << "Server just treated the move " << move_str << endl;
//        if ((has_won = board.gameFinished()) != 0)
//        {
//            end_game = true;
//            cout << (has_won == 1?"White":"Black") << " player has won the game." << endl;
//        }
//    }

//    socket.close();
//    socketB.close();
//    cout << "Terminating server thread." << endl;
}

