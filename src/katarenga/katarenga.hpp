#ifndef KATARENGA_HPP
#define KATARENGA_HPP

// Standard Library
#include <string>

struct MainArguments
{
    bool is_standalone;             // Whether this is a standalone game process.
//    bool is_server;                 // Whether this is a server game process
//    bool is_player;                 // Whether this is a player game process
    std::string server_ip;          // The ip of the server.
    int server_white_port;          // The server socket port for the white player.
    int server_black_port;          // The server socket port for the black player.
    int graphics_port;              // The socket port of the graphic thread. Only for client processes.
    int player;                     // Whether this process is White (1) Black (-1), or Server (0).
    bool verbose;                   // Whether to be verbose or not.
};

extern MainArguments MainArguments;

#endif // KATARENGA_HPP