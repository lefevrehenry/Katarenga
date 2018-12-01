#ifndef KATARENGA_HPP
#define KATARENGA_HPP

#include <string>

struct MainArguments
{
    bool is_server;                 // Whether this is the server-side or client-side.
    std::string server_ip;          // The ip of the server.
    int server_player_port;         // The server port for this player.
    int server_opponent_port;       // The server port of the other player. Only used for server initialization.
    int graphics_port;              // The socket port of the graphic thread.
    bool verbose;                   // Whether to be verbose.
    int player;                     // Whether this process is White (1) or Black (-1)
};

int parse_main_args(int argc, char * argv[], MainArguments & main_args);





#endif // KATARENGA_HPP
