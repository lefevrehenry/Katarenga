#ifndef SERVER_UTILS_HPP
#define SERVER_UTILS_HPP

#include <common/shared_utils.hpp>
#include <common/board/board_utils.hpp>

// Standard Library
#include <string>

struct ServerInfo
{
    int server_white_port;          // The server socket port for the white player
    int server_black_port;          // The server socket port for the black player
    //int server_publish_port;        // The server socket port to publish message

    std::string white_binding_point; // The binding point to communicate with white player
    std::string black_binding_point; // The binding point to communicate with black player

    bool verbose;                   // Whether to be verbose or not
};


BoardPlayer gameActorToPlayer(const GameActor& actor);

void server_msg(const std::string& msg);

#endif // SERVER_UTILS_HPP
