#include "katarenga.hpp"
#include "player.hpp"
#include "standalone.hpp"

#include <iostream>
#include <map>

#include "docopt/docopt.h"
#include <GLFW/glfw3.h>

struct MainArguments MainArguments;

int parse_main_args(int argc, char * argv[])
{
    static const char usage[] =
R"(Katarenga: A nice two-player board game!

Usage:
    katarenga                   [options]
    katarenga --server          [options]
    katarenga --white | --black [--server-ip <ip>]
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

    std::map<std::string, docopt::value> args = docopt::docopt(usage, {argv+1, argv+argc}, true);

    int offset_port = args["--offset-port"].asLong();
//    int graphics_white_port = offset_port;
//    int server_white_port = offset_port + 1;
//    int graphics_black_port = offset_port + 2;
//    int server_black_port = offset_port + 3;

    MainArguments.verbose = args["--verbose"].asBool();
//    if (args["--server"].asBool())
//    {
//        MainArguments.is_server = true;
//        MainArguments.player = 0;
//        MainArguments.server_ip = args["--server-ip"].asString();
//        MainArguments.server_white_port = offset_port + 1;
//        MainArguments.server_black_port = offset_port + 3;
//    }
    if (args["--white"].asBool())
    {
//        MainArguments.is_player = true;
        MainArguments.player = 1;
        MainArguments.server_ip = args["--server-ip"].asString();
        MainArguments.graphics_port = offset_port;
        MainArguments.server_white_port = offset_port + 1;
    }
    else if (args["--black"].asBool())
    {
//        MainArguments.is_player = true;
        MainArguments.player = -1;
        MainArguments.server_ip = args["--server-ip"].asString();
        MainArguments.graphics_port = offset_port + 2;
        MainArguments.server_black_port = offset_port + 3;
    }
//    else
//    {
//        // This is the standalone version
//        MainArguments.is_standalone = true;
//        MainArguments.graphics_port = offset_port;
//    }

    MainArguments.is_standalone = args["--server-ip"].asString().empty();

    if(MainArguments.verbose)
    {
        std::cout << "Parsing argument successfully done!" << std::endl;
    }

    return 0;
}


int main(int argc, char * argv[])
{
    // Let's parse the command-line arguments!
    if (parse_main_args(argc, argv))
        return 1;

//    if(MainArguments.is_standalone)
//    {
//        // TODO Create a standalone version with server and graphics processes
//        standalone_function(MainArguments.graphics_port, MainArguments.verbose);
//    }
//    else if(MainArguments.is_server)
//    {
//        server_function(MainArguments.server_white_port, MainArguments.server_black_port, MainArguments.verbose);
//    }
//    else
    {
//        std::string server_endpoint;
//        if (MainArguments.player == 1)
//        {
//            server_endpoint = "tcp://" + MainArguments.server_ip + ":" + std::to_string(MainArguments.server_white_port);
//        }
//        else
//        {
//            server_endpoint = "tcp://" + MainArguments.server_ip + ":" + std::to_string(MainArguments.server_black_port);
//        }
//        player_function(MainArguments.player, MainArguments.graphics_port, server_endpoint, MainArguments.verbose);
        player_function();
    }

    return 0;
}
