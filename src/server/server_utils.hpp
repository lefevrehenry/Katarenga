#ifndef UTILS_HPP
#define UTILS_HPP

#include "Cell.hpp"

// ZMQ
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <string>

using Move = std::pair<const Cell&,const Cell&>;

struct ServerInfo
{
    int server_white_port;          // The server socket port for the white player
    int server_black_port;          // The server socket port for the black player
    //int server_publish_port;        // The server socket port to publish message

    std::string white_binding_point; // The binding point to communicate with white player
    std::string black_binding_point; // The binding point to communicate with black player

    bool verbose;                   // Whether to be verbose or not
};


/////////////////////////////////////////////////

/**
 * @brief return a zmqpp::message matching the corresponding reply of the request
 * @param input request_message from which the reply is constructed
 * @return reply excepted for the request_message
 */
//zmqpp::message process_request(zmqpp::message& request_message);

/**
 * @brief return a zmqpp::message matching the corresponding broadcast of the request
 * @param input request_message from which the broadcast is constructed
 * @param input reply_message from which the broadcast is constructed
 * @return broadcast expected for the request_message
 */
//zmqpp::message process_broadcast(zmqpp::message& request_message, zmqpp::message& reply_message);

// Generates a Sring containing the cell types of the board, row by row.
std::string generateBoardCellTypes();

void server_msg(const std::string& msg);

#endif // UTILS_HPP
