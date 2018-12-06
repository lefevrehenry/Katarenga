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
    Board board(verbose);
    string s_player = this_player == 1 ? "White" : "Black";

    cout << "I'm main process of " << s_player << " " << this_player << endl;


    zmq::context_t contextGL(1);
    zmq::socket_t socketGL(contextGL, ZMQ_PAIR);
    socketGL.bind("tcp://*:"+to_string(graphics_port));
    std::thread thr_GL(graphics_function, this_player, graphics_port, verbose);

    zmq::context_t contextS(1);
    zmq::socket_t socketServer(contextS, ZMQ_PAIR);
    socketServer.connect(server_endpoint);

    zmq::message_t m;
    socketGL.recv(&m);
    socketServer.send(m);

    socketServer.recv(&m);
    socketGL.send(m);

    cout << s_player <<  " player relayed message from GL thread to server" << endl;

    thr_GL.join();
    cout << s_player << " player terminating main process" << endl;

    socketServer.close();
    socketGL.close();
}
