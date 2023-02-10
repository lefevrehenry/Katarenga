#ifndef UTILS_HPP
#define UTILS_HPP

#include <common/board/board_utils.hpp>

#include <zmqpp/zmqpp.hpp>
#include <string>
#include <stdexcept>

struct GameSettings
{
    bool is_standalone;             // Whether this is a standalone client process.
    std::string server_ip;          // The ip of the server.
    int server_port;                // The socket port of the server.
    int graphics_port;              // The socket port of the graphics thread.
    bool verbose;                   // Whether to be verbose or not.
    BoardPlayer self_player;
    std::string server_binding_point; // The binding point to communicate with the server
    std::string render_binding_point; // The binding point to communicate with the render thread
};

extern bool verbose; // UGLY hack for now...

/* Misc functions */
void render_msg(const std::string& msg);
void player_msg(const std::string& msg);

void convert_move_str(const std::string& move_str, int& source, int& dest);
std::string create_move_str(int source, int dest);

#endif // UTILS_HPP

