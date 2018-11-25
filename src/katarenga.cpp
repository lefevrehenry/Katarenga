#include "katarenga.hpp"
#include "Board.hpp"

#include <iostream>
#include <map>

#include "docopt/docopt.h"
#include "zmq.hpp"

using namespace std;

int parse_main_args(int argc, char * argv[], MainArguments & main_args)
{
    static const char usage[] =
R"(Katarenga: A nice two-player board game!.

Usage:
    katarenga (--server | --client) [--server-ip <ip>]
                                    [--server-port <port>]
                                    [options]
    katarenga -h | --help

Input options:
    --server                       This process will be server
    --client                       This process will be client
    --server-ip <ip>               If client process, the IP address of the server.
                                   If server process, this option is ignored.
                                   [default: 127.0.0.1]
    --server-port <port>           The port number of the server.
                                   [default: 28000]

Other options:
    -v, --verbose                  Makes katarenga verbose.
    -h, --help                     Shows this help.
)";

    map<string, docopt::value> args = docopt::docopt(usage, {argv+1, argv+argc}, true);

    std::string server_ip;
    if (args["--server"].asBool())
    {
        main_args.is_server = true;
        server_ip = "*";
    }
    else if (args["--client"])
    {
        main_args.is_server = false;
        server_ip = args["--server-ip"].asString();
    }
    else
    {
        cout << "Argument parsing error, please specify whether the process is a server or a client" << endl;
        return 1;
    }

    int server_port = args["--server-port"].asLong();
    main_args.socket_endpoint = "tcp://" + server_ip + ":" + to_string(server_port);
    main_args.verbose = args["--verbose"].asBool();

            cout << "Parsing argument successfully done!\nI'm a Katarenga " << (main_args.is_server?"server":"client")
              << " and my socket endpoint is " << main_args.socket_endpoint << endl;
    return 0;
}


int main(int argc, char * argv[])
{
    // Let's parse the command-line arguments!
    MainArguments main_args;
    if (parse_main_args(argc, argv, main_args))
    {
        return 1;
    }

    cout << "HELLO!" << endl;

    if (main_args.is_server)
    {
        zmq::context_t context(1);
        zmq::socket_t socket(context, ZMQ_REP);
        socket.bind(main_args.socket_endpoint);

        zmq::message_t m;
        socket.recv(&m);
        string s((char*)m.data(), m.size());
        cout << "Server received " << s.c_str() << m.size();
    }
    else
    {
        zmq::context_t context(1);
        zmq::socket_t socket (context, ZMQ_REQ);
        socket.connect(main_args.socket_endpoint);
        cout << "Client connected! Sending message" << endl;

        //zmq::message_t m(4);
        string s = "toto";
        socket.send(s.data(), s.size());
    }

    //Board board;
    //board.main_loop();

    return 0;
}
