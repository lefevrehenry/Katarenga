#include "server.hpp"
#include "Board.hpp"
#include "server_utils.hpp"

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// docopt
#include <docopt/docopt.h>

// Standard Library
#include <iostream>
#include <map>
#include <string>

int parse_main_args(int argc, char * argv[])
{
    static const char usage[] =
R"(Katarenga-Server: A nice two-player board game!

Usage:
    server                      [options]
    server -h | --help

Input options:
    --offset-port <port>        The offset on which socket port will be based on.
                                [default: 28000]

Other options:
    -v, --verbose                  Makes katarenga verbose.
    -h, --help                     Shows this help.
)";

    std::map<std::string, docopt::value> args = docopt::docopt(usage, {argv+1, argv+argc}, true);

    int offset_port = args["--offset-port"].asLong();
    bool verbose = args["--verbose"].asBool();

    ServerInfo.server_white_port = offset_port;
    ServerInfo.server_black_port = offset_port + 1;
    ServerInfo.server_publish_port = offset_port + 2;
    ServerInfo.verbose = verbose;
    ServerInfo.board = nullptr;

    return 0;
}

void server_function()
{
    int server_white_port = ServerInfo.server_white_port;
    int server_black_port = ServerInfo.server_black_port;
    int server_publish_port = ServerInfo.server_publish_port;

    zmqpp::context context;

    // Open sockets for White and Black players
    zmqpp::socket white_player_socket(context, zmqpp::socket_type::reply);
    white_player_socket.bind("tcp://*:" + std::to_string(server_white_port));

    zmqpp::socket black_player_socket(context, zmqpp::socket_type::reply);
    black_player_socket.bind("tcp://*:" + std::to_string(server_black_port));

    // Open socket for publish zmq message to both client and spectators
    zmqpp::socket server_publish_socket(context, zmqpp::socket_type::publish);
    server_publish_socket.bind("tcp://*:" + std::to_string(server_publish_port));

    // Create a poller for any request received
    zmqpp::poller poller;

    poller.add(white_player_socket, zmqpp::poller::poll_in);
    poller.add(black_player_socket, zmqpp::poller::poll_in);

    // Setup game
    Board board;
    generateBoard(&board);

    // Give a global access to the board
    ServerInfo.board = &board;

    bool end_game = board.gameFinished();
    int has_won = 0;

    // loop until the game is finished
    while (!end_game) {

        board.print();

        // wait for a request
        if(!poller.poll(zmqpp::poller::wait_forever)) {

            // if a message is received from the white player
            if(poller.has_input(white_player_socket)) {
                // pull the request message
                zmqpp::message request_message;
                white_player_socket.receive(request_message);

                // construct and push the reply message
                zmqpp::message reply_message = process_request(request_message);
                white_player_socket.send(reply_message);

                // construct and publish the broadcast message
                zmqpp::message publish_message = process_broadcast(request_message, reply_message);
                server_publish_socket.send(publish_message);
            }

            // if a message is received from the black player
            if(poller.has_input(black_player_socket)) {
                // pull the request message
                zmqpp::message request_message;
                black_player_socket.receive(request_message);

                // construct and push the reply message
                zmqpp::message reply_message = process_request(request_message);
                black_player_socket.send(reply_message);

                // construct and publish the broadcast message
                zmqpp::message publish_message = process_broadcast(request_message, reply_message);
                server_publish_socket.send(publish_message);
            }

            // terminate the loop if someone has won
            if ((has_won = board.gameFinished()) != 0) {
                end_game = true;
                std::cout << (has_won == 1 ? "White" : "Black") << " player has won the game." << std::endl;
            }
        }
    }

//    // Send the board configuration to players and recv ACKs
//    if(verbose){ cout << "Sending board configuration to players" << endl; }
////    s_send(socket, board_configuration);
////    s_send(socketB, board_configuration);
////    s_recv(socket);
////    s_recv(socketB);

//    // Main loop
//    string move_str;
//    bool end_game = false;
//    int has_won = 0;
//    while (!end_game)
//    {
//        board.print();

//        // TODO put all this in a generic function depending on the current player?
//        if(board.getCurrentPlayer() == 1)
//        {
//            // Waiting for White to send his move
//            move_str = s_recv(socket);
//            if(!board.isValidMove(move_str, 1))
//            {
//                s_send(socket, "reject");
//            }
//            else
//            {
//                board.playMove(move_str);
//                s_send(socket, move_str);
//                s_send(socketB, move_str);
//            }
//        }
//        else
//        {
//            move_str = s_recv(socketB);
//            if(!board.isValidMove(move_str, -1))
//            {
//                s_send(socketB, "reject");
//            }
//            else
//            {
//                board.playMove(move_str);
//                s_send(socket, move_str);
//                s_send(socketB, move_str);
//            }
//        }

//        cout << "Server just treated the move " << move_str << endl;
//        if ((has_won = board.gameFinished()) != 0)
//        {
//            end_game = true;
//            cout << (has_won == 1?"White":"Black") << " player has won the game." << endl;
//        }
//    }

    white_player_socket.close();
    black_player_socket.close();
    std::cout << "Terminating server thread." << std::endl;
}

int main(int argc, char* argv[])
{
    // Let's parse the command-line arguments!
    if (parse_main_args(argc, argv))
        return 1;

    server_function();

    return 0;
}
