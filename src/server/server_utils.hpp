#ifndef UTILS_HPP
#define UTILS_HPP

// ZMQ
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <string>

class Board;
class Cell;

using Move = std::pair<Cell*,Cell*>;

struct ServerInfo
{
    int server_white_port;          // The server socket port for the white player
    int server_black_port;          // The server socket port for the black player
    int server_publish_port;        // The server socket port to publish message
    bool verbose;                   // Whether to be verbose or not
    Board* board;                   // pointer to the Board used for the game
};

extern ServerInfo ServerInfo;

/////////////////////////////////////////////////

/**
 * @brief return a zmqpp::message matching the corresponding reply of the request
 * @param input request_message from which the reply is constructed
 * @return reply excepted for the request_message
 */
zmqpp::message process_request(zmqpp::message& request_message);

/**
 * @brief return a zmqpp::message matching the corresponding broadcast of the request
 * @param input request_message from which the broadcast is constructed
 * @param input reply_message from which the broadcast is constructed
 * @return broadcast expected for the request_message
 */
zmqpp::message process_broadcast(zmqpp::message& request_message, zmqpp::message& reply_message);

// Generates a Sring containing the cell types of the board, row by row.
void generateBoard(Board* board);

#endif // UTILS_HPP
