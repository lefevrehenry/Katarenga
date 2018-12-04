#include "network_utils.hpp"
#include "server.hpp"
#include "Board.hpp"

#include <iostream>

using namespace std;
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

    if(verbose) { cout << "I'm the server and my two sockets are open" << endl; }

    Board server_board(verbose);
    bool end_game = false;
    while (!end_game)
    {
        string s = s_recv(socketW);
        cout << "Server received: " << s << endl;

        string s2 = s_recv(socketB);
        cout << "Server received: " << s2 << endl;

        s_send(socketB, s);
        s_send(socketW, s2);

        end_game = true;
    }

    socketW.close();
    socketB.close();
    cout << "Terminating server thread." << endl;
}

