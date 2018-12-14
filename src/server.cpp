#include "network_utils.hpp"
#include "server.hpp"
#include "Board.hpp"

#include <iostream>

using std::cout, std::endl, std::string, std::to_string;
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

    if(verbose){ cout << "Waiting for a Hello from White process" << endl; }
    s_recv(socketW);
    if(verbose){ cout << "Waiting for a Hello from Black process" << endl; }
    s_recv(socketB);

    // Setup game
    string board_configuration = "";
    Board board(board_configuration, verbose);

    // Send the board configuration to players
    s_send(socketW, board_configuration);
    s_send(socketB, board_configuration);

    // Main loop
    int current_player = board.getCurrentPlayer();
    string move_str;
    bool end_game = false;
    while (!end_game)
    {
        bool invalid_move;

        // Get move from White
        move_str = s_recv(socketW);
        invalid_move = !board.isValidMove(move_str, current_player);
        while(invalid_move)
        {
            s_send(socketW, "reject");
            move_str = s_recv(socketW);
            invalid_move = !board.isValidMove(move_str, current_player);
        }

        if (verbose){ cout << "Server received valid move " << move_str << " from White" << endl; }
        board.movePiece(move_str);
        current_player = -current_player;


        // Get move from Black


        string s2 = s_recv(socketB);
        cout << "Server received: " << s2 << " from Black" << endl;
;

        end_game = true;
    }

    socketW.close();
    socketB.close();
    cout << "Terminating server thread." << endl;
}

