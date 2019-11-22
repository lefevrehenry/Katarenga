#include "network_utils.hpp"
#include "graphics.hpp"
#include "Board.hpp"

#include <iostream>

using std::cout;        // At some point
using std::cin;         // We'll need to
using std::endl;        // Get rid of
using std::string;      // These
using std::to_string;   // Declarations

using namespace zmq;


/*bool polling(socket_t & socket, int timeout)
{
    zmq::pollitem_t items [] = {{ socket, 0, ZMQ_POLLIN, 0}};
    int n = 0;
    while (1)
    {
        zmq::poll(&items[0], 1, timeout);
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
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PAIR);
    socket.connect(socket_endpoint);

    string board_configuration = s_recv(socket);
    s_send(socket, "ACK");

    cout << "GL received board config:\n" << board_configuration << endl;

    Board board(board_configuration, verbose);

    cout << "GL thread ready to play!" << endl;

    // Main loop
    string move_str;
    bool end_game = false;
    int has_won = 0;
    while(!end_game)
    {
        if (board.getCurrentPlayer() == this_player)
        {
            // It's our turn
            cout << "Enter your string move: ";
            cin >> move_str;

            s_send(socket, move_str);
            /*if(!polling(socket, 5))
            {
                // TODO throw an error
                cout << "ERROR WHILE POLLING, terminating GL thread." << endl;
                socket.close();
                return;
            }*/
            string ret = s_recv(socket);
            if (ret == "accept")
            {
                board.playMove(move_str);
            }
            else if (ret == "reject")
            {
                cout << "The move was not valid" << endl;
            }
            else
            {
                cout << "ERROR: Weird message received from player thread, terminating." << endl;
                socket.close();
                return;
            }
        }
        else
        {
            // It's the opponent turn
            /*if(!polling(socket, 5))
            {
                // TODO throw an error
                cout << "ERROR WHILE POLLING, terminating GL thread." << endl;
                socket.close();
                return;
            }*/
            move_str = s_recv(socket);
            board.playMove(move_str);
            cout << "Other player just played " << move_str << endl;
        }

        if ((has_won = board.gameFinished()) != 0)
        {
            end_game = true;
            cout << "Woah! " << (has_won == 1?"White":"Black") << " player has won the game!"
                                                                  "" << endl;
        }
    }


    socket.close();
    cout << "Terminating GL thread." << endl;
}
