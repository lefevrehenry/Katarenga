#include "katarenga.hpp"
#include "player.hpp"
#include "standalone.hpp"

#include <iostream>
#include <map>

#include "docopt/docopt.h"
#include <GLFW/glfw3.h>

bool verbose; // UGLY hack for now...

int parse_main_args(int argc, char * argv[], MainArguments &main_args)
{
    static const char usage[] =
R"(Katarenga: A nice two-player board game!

Usage:
    katarenga --white | --black    [--server-ip <ip>]
                                   [--offset-port <port>]
                                   [options]
    katarenga --standalone         [options]
    katarenga -h | --help

Input options:
    --server-ip <ip>               The IP address of the server.
                                   [default: 127.0.0.1]
    --server-port <port>           The server port for the socket.
                                   [default: 28000]

Other options:
    -v, --verbose                  Makes katarenga verbose.
    -h, --help                     Shows this help.
)";

    std::map<std::string, docopt::value> args = docopt::docopt(usage, {argv+1, argv+argc}, true);

    main_args.server_ip = args["--server-ip"].asString();
    main_args.server_port = args["--server-port"].asLong();

    main_args.verbose = args["--verbose"].asBool();

    if (args["--standalone"].asBool())
    {
        main_args.is_standalone = true;
        main_args.self_player = 1;
    }
    else if (args["--white"].asBool())
    {
        main_args.is_standalone = false;
        main_args.self_player = 1;
    }
    else if (args["--black"].asBool())
    {
        main_args.is_standalone = false;
        main_args.self_player = -1;
    }
    else
    {
        std::cout << "Error while parsing arguments, this should not happen" << std::endl;
        return 1;
    }

    return 0;
}


int main(int argc, char * argv[])
{
    // Let's parse the command-line arguments!
    MainArguments main_args;
    if (parse_main_args(argc, argv, main_args))
        return 1;

    verbose = main_args.verbose;
    player_msg("Arguments successfully parsed!");

    if(main_args.is_standalone)
    {
        // Create a thread running the server process
        /*if(main_args.verbose)
            player_msg("Starting the server thread");

        std::thread server_thread(server_function);

        // Then launch the standalone function
        if(main_args.verbose)
            player_msg("Starting the main function as standalone");
        standalone_function(main_args);

        if(main_args.verbose)
            player_msg("Waiting termination of the server thread");
        server_thread.join(main_args);*/
        throw std::runtime_error("Standalone feature not implemented yet");
    }
    else
    {
        player_msg("Creating Player and starting main loop");

        Player player(main_args);
        player.loop();
    }

    return 0;
}
