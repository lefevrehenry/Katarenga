#include "katarenga.hpp"
#include "Board.hpp"
#include "network_utils.hpp"
#include "server.hpp"
//#include "graphics.hpp"

#include <iostream>
#include <map>
#include <thread>

#include "docopt/docopt.h"

using namespace std;

int parse_main_args(int argc, char * argv[], MainArguments & main_args)
{
    static const char usage[] =
R"(Katarenga: A nice two-player board game!.

Usage:
    katarenga (--white | --black)   [--server-ip <ip>]
                                    [--server-port <port>]
                                    [--graphics-port <port>]
                                    [options]
    katarenga -h | --help

Input options:
    --white                        This process will be the White player.
                                   For the moment, this process will also hold the server thread.
    --black                        This process will be the Black player.
    --server-ip <ip>               If client process, the IP address of the server.
                                   If server process, this option is ignored.
                                   [default: 127.0.0.1]
    --offset-port <port>           The offset on which socket port will be based on.
                                   [default: 28000]

Other options:
    -v, --verbose                  Makes katarenga verbose.
    -h, --help                     Shows this help.
)";

    map<string, docopt::value> args = docopt::docopt(usage, {argv+1, argv+argc}, true);

    int offset_port = args["--offset-port"].asLong();
    int graphics_white_port = offset_port;
    int server_white_port = offset_port + 1;
    int graphics_black_port = offset_port + 2;
    int server_black_port = offset_port + 3;;

    if (args["--white"].asBool())
    {
        main_args.player = 1;
        main_args.is_server = true;
        main_args.graphics_port = offset_port;
        main_args.server_player_port = offset_port + 1;
        main_args.server_opponent_port = offset_port + 3;
    }
    else if (args["--black"].asBool())
    {
        main_args.player = -1;
        main_args.is_server = false;
        main_args.graphics_port = offset_port + 2;
        main_args.server_player_port = offset_port + 3;
    }
    else
    {
        cout << "Argument parsing error, please specify whether the process is a server or a client" << endl;
        return 1;
    }

    main_args.server_ip = args["--server-ip"].asString();
    main_args.verbose = args["--verbose"].asBool();

    if(main_args.verbose)
    {
        cout << "Parsing argument successfully done!\nI'm a Katarenga " << (main_args.player==1?"White":"Black")
          << " player, server endpoint: " << main_args.server_ip << ":" << main_args.server_player_port
          << ", graphics port: " << main_args.graphics_port << endl;
    }
    return 0;
}



// TODO move this function in another file
void GL_function(int this_player, int socket_port)
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

int main(int argc, char * argv[])
{
    // Let's parse the command-line arguments!
    MainArguments main_args;
    if (parse_main_args(argc, argv, main_args))
    {
        return 1;
    }
    Board board(main_args.verbose);
    int i_player = main_args.player;
    string s_player = main_args.player == 1 ? "White" : "Black";

    cout << "I'm main thread of " << s_player << " " << i_player << endl;

    ///////////////////////////////////////////
    // TODO put this in an init network/threads function

    // If I'm holding the server, launch it
    std::thread thr_server;
    if (main_args.is_server)
    {
        std::thread thr(server_function, main_args.server_player_port, main_args.server_opponent_port, main_args.verbose);
        thr_server.swap(thr);
    }

    zmq::context_t contextGL(1);
    zmq::socket_t socketGL(contextGL, ZMQ_PAIR);
    std::thread thr_GL(GL_function, main_args.player, main_args.graphics_port);
    socketGL.bind("tcp://*:"+to_string(main_args.graphics_port));

    zmq::context_t contextS(1);
    zmq::socket_t socketServer(contextS, ZMQ_PAIR);
    string server_endpoint = "tcp://" + main_args.server_ip + ":" + to_string(main_args.server_player_port);;
    socketServer.connect(server_endpoint);

    zmq::message_t m;
    socketGL.recv(&m);
    socketServer.send(m);

    socketServer.recv(&m);
    socketGL.send(m);

    cout << s_player <<  " player relayed message from GL thread to server" << endl;

    thr_GL.join();
    if(main_args.is_server)
    {
        thr_server.join();
    }
    cout << s_player << " player terminating main process" << endl;

    socketServer.close();
    socketGL.close();
    return 0;
}
