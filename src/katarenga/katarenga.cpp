#include "katarenga.hpp"
#include "player.hpp"
#include "standalone.hpp"

#include <iostream>
#include <map>

#include "docopt/docopt.h"
#include <GLFW/glfw3.h>

bool verbose; // UGLY hack for now...

int parse_arguments(int argc, char * argv[], GameSettings &game_settings)
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
                                   Ignored in standalone mode.
                                   [default: 127.0.0.1]
    --server-port <port>           The server port for the socket.
                                   Ignored in standalone mode.
                                   [default: 28000]

Other options:
    -v, --verbose                  Makes Katarenga verbose.
    -h, --help                     Shows this help.
)";

    std::map<std::string, docopt::value> args = docopt::docopt(usage, {argv+1, argv+argc}, true);



    game_settings.verbose = args["--verbose"].asBool();

    if (args["--standalone"].asBool())
    {
        game_settings.is_standalone = true;
        game_settings.self_player = 0;
        game_settings.server_binding_point = "inproc://katarenga-server-thread";
    }
    else
    {
        if (args["--white"].asBool())
        {
            game_settings.is_standalone = false;
            game_settings.self_player = 1;
        }
        else if (args["--black"].asBool())
        {
            game_settings.is_standalone = false;
            game_settings.self_player = -1;
        }

        game_settings.server_ip = args["--server-ip"].asString();
        game_settings.server_port = args["--server-port"].asLong();

        game_settings.server_binding_point = "tcp://" + game_settings.server_ip + ":" + std::to_string(game_settings.server_port);
    }

    game_settings.render_binding_point = "inproc://katarenga-render-thread";


    return 0;
}


int main(int argc, char * argv[])
{
    // Let's parse the command-line arguments!
    GameSettings game_settings;
    if (parse_arguments(argc, argv, game_settings))
        return 1;

    verbose = game_settings.verbose;
    player_msg("Arguments successfully parsed!");

    if(game_settings.is_standalone)
    {
        // Create a thread running the server process
        /*if(game_settings.verbose)
            player_msg("Starting the server thread");

        std::thread server_thread(server_function);

        // Then launch the standalone function
        if(game_settings.verbose)
            player_msg("Starting the main function as standalone");
        standalone_function(game_settings);

        if(game_settings.verbose)
            player_msg("Waiting termination of the server thread");
        server_thread.join(game_settings);*/
        throw std::runtime_error("Standalone feature not implemented yet");
    }
    else
    {
        player_msg("Creating Player and starting main loop");

        Player player(game_settings);
        player.loop();
    }

    return 0;
}
