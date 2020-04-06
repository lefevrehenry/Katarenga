#ifndef UTILS_HPP
#define UTILS_HPP

#include <zmqpp/zmqpp.hpp>
#include <string>
#include <stdexcept>

struct MainArguments
{
    bool is_standalone;             // Whether this is a standalone client process.
    std::string server_ip;          // The ip of the server.
    int server_port;                // The socket port of the server.
    int graphics_port;              // The socket port of the graphics thread.
    bool verbose;                   // Whether to be verbose or not.
    int self_player;                // 1 for White; -1 for Black;
};

extern bool verbose; // UGLY hack for now...

/* Misc functions */
void render_msg(const std::string& msg);
void player_msg(const std::string& msg);

#endif // UTILS_HPP

