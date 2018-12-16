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
    zmq::context_t context(1);
    zmq::socket_t socketGL(context, ZMQ_PAIR);
    socketGL.bind("tcp://*:"+to_string(graphics_port));
    std::thread thr_GL(graphics_function, this_player, graphics_port, verbose);

    zmq::socket_t socketServer(context, ZMQ_PAIR);
    socketServer.connect(server_endpoint);

    // Receive the board configuration from the server
    std::string board_configuration = s_recv(socketServer);
    s_send(socketServer, "ACK");

    cout << "RECEIVED board config:\n" << board_configuration << endl;

    // Forward the board configuration to the graphics thread and wait for ACK.
    s_send(socketGL, board_configuration);
    s_recv(socketGL);

    cout << "just sent board config to graphics thread" << endl;

    // Main loop of the game
    Board board(board_configuration, verbose);

    cout << "Client process ready to play!" << endl;

    string move_str;
    string ret;
    bool end_game = false;
    while(!end_game)
    {
        board.print();

        if (board.getCurrentPlayer() == this_player)
        {
            move_str = s_recv(socketGL);
            if (board.isValidMove(move_str, this_player))
            {
                s_send(socketServer, move_str);
                ret = s_recv(socketServer);
                if (ret == move_str)
                {
                    // Move was validated by server
                    board.playMove(move_str);
                    s_send(socketGL, "accept");
                }
                else if (ret == "reject")
                {
                    s_send(socketGL, "reject");
                }
                else
                {
                    //Throw error
                    cout << "ERROR: server sent '" << ret << "' back but reject or move '" << move_str << "' was expected." << endl;
                    pthread_cancel(thr_GL.native_handle()); // This should do what I want: kill the thread
                    return;
                }
            }
            else
            {
                // Move sent by the graphics thread is not valid
                s_send(socketGL, "reject");
            }
        }
        else
        {
            // Other player's turn, waiting for message from server
            move_str = s_recv(socketServer);
            board.playMove(move_str);
            s_send(socketGL, move_str);
        }

        if (board.gameFinished() != 0)
        {
            end_game = true;
        }
    }

    // Clean up and terminate
    thr_GL.join();

    socketServer.close();
    socketGL.close();
}
