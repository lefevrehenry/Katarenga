#include "network_utils.hpp"
#include "server.hpp"
#include "Board.hpp"

#include <iostream>

using std::cout;        // At some point we'll
using std::endl;        // Need to get rid of
using std::string;      // These
using std::to_string;   // Declarations

using namespace zmq;

void server_function(int socket_white_port, int socket_black_port, bool verbose)
{
    // Open sockets for White and Black players
    zmq::context_t contextW(1);
    zmq::socket_t socketW(contextW, ZMQ_PAIR);
    socketW.bind("tcp://*:"+to_string(socket_white_port));

    zmq::context_t contextB(1);
    zmq::socket_t socketB(contextB, ZMQ_PAIR);
    socketB.bind("tcp://*:"+to_string(socket_black_port));

    // Setup game
    string board_configuration = "";
    Board board(board_configuration, verbose);


    // Send the board configuration to players and recv ACKs
    if(verbose){ cout << "Sending board configuration to players" << endl; }
    s_send(socketW, board_configuration);
    s_send(socketB, board_configuration);
    s_recv(socketW);
    s_recv(socketB);

    // Main loop
    string move_str;
    bool end_game = false;
    int has_won = 0;
    while (!end_game)
    {
        board.print();

        // TODO put all this in a generic function depending on the current player?
        if(board.getCurrentPlayer() == 1)
        {
            // Waiting for White to send his move
            move_str = s_recv(socketW);
            if(!board.isValidMove(move_str, 1))
            {
                s_send(socketW, "reject");
            }
            else
            {
                board.playMove(move_str);
                s_send(socketW, move_str);
                s_send(socketB, move_str);
            }
        }
        else
        {
            move_str = s_recv(socketB);
            if(!board.isValidMove(move_str, -1))
            {
                s_send(socketB, "reject");
            }
            else
            {
                board.playMove(move_str);
                s_send(socketW, move_str);
                s_send(socketB, move_str);
            }
        }

        cout << "Server just treated the move " << move_str << endl;
        if ((has_won = board.gameFinished()) != 0)
        {
            end_game = true;
            cout << (has_won == 1?"White":"Black") << " player has won the game." << endl;
        }
    }

    socketW.close();
    socketB.close();
    cout << "Terminating server thread." << endl;
}

