#include "network_utils.hpp"
#include "standalone.hpp"
#include "graphics.hpp"
#include "Board.hpp"

#include <iostream>
#include <thread>

#include <zmqpp/zmqpp.hpp>

using std::cout;        // At some point we'll
using std::endl;        // Need to get rid of
using std::string;      // These
using std::to_string;   // Declarations

void standalone_function(const int graphics_port, const bool verbose)
{
    // Open socket for graphics thread
    zmqpp::context context;
    zmqpp::socket socketGL(context, zmqpp::socket_type::pair);
    socketGL.bind("tcp://*:"+to_string(graphics_port));
    std::thread thr_GL(standalone_graphics_function, graphics_port, verbose);

    // Setup game
    string board_configuration = "";
    Board board(board_configuration, verbose);


    // Send the board configuration to graphics thread and recv ACK
    if(verbose){ cout << "Sending board configuration to graphics thread" << endl; }
    s_send(socketGL, board_configuration);
    s_recv(socketGL);

    // Main loop
    string move_str;
    string mesg;
    bool end_game = false;
    int has_won = 0;            // 1 if White won, -1 if Black won
    int current_player = 1;


    while (!end_game)
    {
        // Send which player has to play
        mesg = (current_player == 1 ? "p1" : "p2");
        s_send(socketGL, mesg);

        // Wait for the move
        move_str = s_recv(socketGL);

        if (!board.isValidMove(move_str, current_player))
        {
            s_send(socketGL, "reject");
        }
        else
        {
            board.playMove(move_str);
            s_send(socketGL, move_str);
            current_player = -current_player;
        }

        if(verbose) {cout << "Server just treated the move " << move_str << endl;}

        if ((has_won = board.gameFinished()) != 0)
        {
            end_game = true;
            cout << (has_won == 1?"White":"Black") << " player has won the game." << endl;

            // Notifies the graphics thread that the game is finished
            mesg = (has_won == 1 ? "w1" : "w2");
            s_send(socketGL, mesg);
        }
    }

    thr_GL.join();
    //socketGL.close();
    cout << "Terminating server thread." << endl;
}

