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

extern MainArguments MainArguments;

/* Network-related functions */
//  Receive message from socket and convert into string
std::string s_recv(zmqpp::socket& socket);

//  Convert string to message and send to socket
//bool s_send(zmqpp::socket& socket, const std::string& string);
//bool s_send(zmqpp::socket& socket, const char* cstring);


/* Misc functions */
void render_msg(const std::string& msg);
void player_msg(const std::string& msg);

#endif // UTILS_HPP

