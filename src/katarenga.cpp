#include "katarenga.hpp"
#include "Board.hpp"
#include "network_utils.hpp"
#include "server.hpp"
#include "player.hpp"

#include <iostream>
#include <map>

#include "docopt/docopt.h"
#include <GLFW/glfw3.h>

using namespace std;

int parse_main_args(int argc, char * argv[], MainArguments & main_args)
{
    static const char usage[] =
R"(Katarenga: A nice two-player board game!

Usage:
    katarenga (--white | --black | --server)  [--server-ip <ip>]
                                              [--server-port <port>]
                                              [--graphics-port <port>]
                                              [options]
    katarenga -h | --help

Input options:
    --white                        This process will be the client process for the White player.
    --black                        This process will be the client process for the Black player.
    --server                       This process will be the server process.
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

    if (args["--server"].asBool())
    {
        main_args.player = 0;
        main_args.is_server = true;
        main_args.server_white_port = offset_port + 1;
        main_args.server_black_port = offset_port + 3;
    }
    else if (args["--white"].asBool())
    {
        main_args.player = 1;
        main_args.is_server = false;
        main_args.graphics_port = offset_port;
        main_args.server_white_port = offset_port + 1;
    }
    else if (args["--black"].asBool())
    {
        main_args.player = -1;
        main_args.is_server = false;
        main_args.graphics_port = offset_port + 2;
        main_args.server_black_port = offset_port + 3;
    }
    else
    {
        cout << "Argument parsing error, please specify whether the process is a server, white or black player." << endl;
        return 1;
    }

    main_args.server_ip = args["--server-ip"].asString();
    main_args.verbose = args["--verbose"].asBool();

    if(main_args.verbose)
    {
        cout << "Parsing argument successfully done!\nI'm a Katarenga "
             << (main_args.player==1?"White player": (main_args.player==0?"Server":"Black player"))
             << ", the server IP is: " << main_args.server_ip << " and the offset port is " << offset_port << endl;
    }
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

    if(main_args.is_server)
    {
        server_function(main_args.server_white_port, main_args.server_black_port, main_args.verbose);
    }
    else
    {
        string server_endpoint;
        if (main_args.player == 1)
        {
            server_endpoint = "tcp://" + main_args.server_ip + ":" + to_string(main_args.server_white_port);
        }
        else
        {
            server_endpoint = "tcp://" + main_args.server_ip + ":" + to_string(main_args.server_black_port);
        }
        player_function(main_args.player, main_args.graphics_port, server_endpoint, main_args.verbose);
    }

    return 0;
}
