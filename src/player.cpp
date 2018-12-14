#include "network_utils.hpp"
#include "player.hpp"
#include "graphics.hpp"
#include "Board.hpp"

#include <iostream>
#include <thread>

using namespace std;
using namespace zmq;


void player_function(int this_player, int graphics_port, std::string & server_endpoint, bool verbose)
{
    string s_player = this_player == 1 ? "White" : "Black";

    cout << "I'm main process of " << s_player << " " << this_player << endl;

    // Setup the two connection with the graphics thread and the server process
    zmq::context_t contextGL(1);
    zmq::socket_t socketGL(contextGL, ZMQ_PAIR);
    socketGL.bind("tcp://*:"+to_string(graphics_port));
    std::thread thr_GL(graphics_function, this_player, graphics_port, verbose);

    zmq::context_t contextS(1);
    zmq::socket_t socketServer(contextS, ZMQ_PAIR);
    socketServer.connect(server_endpoint);

    // Receive the board configuration from the server
    std::string board_configuration = s_recv(socketServer); //TODO put in a loop and send noACK if size is not equal to 4
    s_send(socketServer, "ACK");

    // Forward the board configuration to the graphics thread and wait for ACK.
    s_send(socketGL, board_configuration);
    s_recv(socketGL);


    // Main loop of the game
    Board board(board_configuration, verbose);


    // If white player
    // Wait for a move from the GL thread
    // While this move is not valid, reject the move

    // Send move to server
    // Wait for ACK or reject
    // Transfer answer to GL thread

    // if reject: loop now
    // else continue

    // Wait now for move of the opponnent from the server
    // Forward it to GL thread



    // Clean up and terminate
    thr_GL.join();

    socketServer.close();
    socketGL.close();
}
