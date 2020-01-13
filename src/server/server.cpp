//#include "network_utils.hpp"
#include "server.hpp"
#include "Board.hpp"

#include <zmqpp/zmqpp.hpp>

#include <docopt/docopt.h>

#include <map>
#include <string>

struct ServerInfo ServerInfo;

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
    ServerInfo.verbose = verbose;

    return 0;
}

void server_function()
{
    zmqpp::context context;

    int server_white_port = ServerInfo.server_white_port;
    int server_black_port = ServerInfo.server_black_port;

    // Open sockets for White and Black players
    zmqpp::socket white_player_socket(context, zmqpp::socket_type::reply);
    white_player_socket.bind("tcp://*:" + std::to_string(server_white_port));

    zmqpp::socket black_player_socket(context, zmqpp::socket_type::reply);
    black_player_socket.bind("tcp://*:" + std::to_string(server_black_port));

    // Setup game
    Board board;

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

//    socket.close();
//    socketB.close();
//    cout << "Terminating server thread." << endl;
}

int main(int argc, char* argv[])
{
    // Let's parse the command-line arguments!
    if (parse_main_args(argc, argv))
        return 1;

    server_function();

    return 0;
}
