#include "katarenga.hpp"
#include "player.hpp"
#include "standalone.hpp"
#include "utils.hpp"

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
    katarenga --white | --black    [--server-ip <ip>]
                                   [--offset-port <port>]
                                   [options]
    katarenga -h | --help

Input options:
    --server-ip <ip>               The IP address of the server.
                                   If the default value is chosen, this will launch
                                   a standalone version of the game.
                                   [default: 127.0.0.1]
    --offset-port <port>           The offset on which socket ports will be based on.
                                   [default: 28000]

Other options:
    -v, --verbose                  Makes katarenga verbose.
    -h, --help                     Shows this help.
)";

    std::map<std::string, docopt::value> args = docopt::docopt(usage, {argv+1, argv+argc}, true);

    int offset_port = args["--offset-port"].asLong();
    MainArguments.graphics_port = offset_port;
    MainArguments.server_port = offset_port + 1;

    MainArguments.verbose = args["--verbose"].asBool();
    MainArguments.is_standalone = args["--server-ip"].asString() == "127.0.0.1";

    if (args["--white"].asBool())
    {
        MainArguments.self_player = 1;
    }
    else if (args["--black"].asBool())
    {
        MainArguments.self_player = -1;
    }
    else
    {
        player_msg("Error: This should not happen");
        return 1;
    }


    if(MainArguments.verbose)
    {
        player_msg("Arguments successfully parsed!");
    }

    return 0;
}


int main(int argc, char * argv[])
{
    // Let's parse the command-line arguments!
    if (parse_main_args(argc, argv))
        return 1;

    if(MainArguments.is_standalone)
    {
        // Create a thread running the server process
        /*if(MainArguments.verbose)
            player_msg("Starting the server thread");

        std::thread server_thread(server_function);

        // Then launch the standalone function
        if(MainArguments.verbose)
            player_msg("Starting the main function as standalone");
        standalone_function();

        if(MainArguments.verbose)
            player_msg("Waiting termination of the server thread");
        server_thread.join();*/
        throw std::runtime_error("Standalone feature not implemented yet");
    }
    else
    {
        player_msg("Starting the main function");

        Player player;
        player.loop();
    }

    return 0;
}
