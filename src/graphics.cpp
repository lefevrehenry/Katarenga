#include "network_utils.hpp"
#include "graphics.hpp"
#include "Board.hpp"

#include <iostream>

using namespace std;
using namespace zmq;

void graphics_function(int this_player, int socket_port, bool verbose)
{
    string socket_endpoint = "tcp://127.0.0.1:" + to_string(socket_port);
    zmq::context_t contextGL(1);
    zmq::socket_t socketGL(contextGL, ZMQ_PAIR);
    socketGL.connect(socket_endpoint);

    string s = "Hello from ";
    s = s + (this_player == 1 ? "White":"Black") + " player GL thread";

    cout << "GL thread sending " << s << endl;

    s_send(socketGL, s);

    s = s_recv(socketGL);

    cout << "GL thread just received " << s << endl;

    socketGL.close();
    cout << "Terminating GL thread." << endl;
}
