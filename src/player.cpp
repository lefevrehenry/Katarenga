#include "network_utils.hpp"
#include "katarenga.hpp"
#include "player.hpp"
#include "graphics.hpp"
#include "server_communication.hpp"

#include <iostream>
#include <thread>
#include <memory>


void player_function()
{
    int this_player = MainArguments.player;
    int graphics_port = MainArguments.graphics_port;
    std::string server_ip = MainArguments.server_ip;
    int server_white_port = MainArguments.server_white_port;
    int server_black_port = MainArguments.server_black_port;
    bool verbose = MainArguments.verbose;

    std::string server_port = std::to_string(this_player == 1 ? server_white_port : server_black_port);

    std::unique_ptr<AbstractServer> server_communication;

    if (MainArguments.is_standalone)
        server_communication.reset(new LocalServer());
    else
        server_communication.reset(new RemoteServer(server_ip, server_port));

    if (!server_communication)
        throw std::runtime_error("enable to start a communication with the server");

    if (!server_communication->checkServerConnectivity())
        throw std::runtime_error("unreacheable server. Check your connectivity or be sure you enter a valid ip:port");

//    std::string server_endpoint;
//    if (this_player == 1)
//    {
//        server_endpoint = "tcp://" + server_ip + ":" + std::to_string(server_white_port);
//    }
//    else
//    {
//        server_endpoint = "tcp://" + server_ip + ":" + std::to_string(server_black_port);
//    }

//    std::string s_player = this_player == 1 ? "White" : "Black";

//    std::cout << "I'm main process of " << s_player << " " << this_player << std::endl;

//    // Setup the two connection with the graphics thread and the server process
//    zmqpp::context context;
//    zmqpp::socket socketGL(context, zmqpp::socket_type::pair);
//    socketGL.bind("tcp://*:"+std::to_string(graphics_port));
//    std::thread thr_GL(graphics_function, this_player, graphics_port, verbose);

////    zmqpp::socket socketServer(context, zmqpp::socket_type::pair);
////    socketServer.connect(server_endpoint);

//    // Receive the board configuration from the server
//    std::string board_configuration = server_communication->getBoardConfiguration();
////    std::string board_configuration = s_recv(socketServer);
////    s_send(socketServer, "ACK");

//    std::cout << "RECEIVED board config:\n" << board_configuration << std::endl;

//    // Forward the board configuration to the graphics thread and wait for ACK.
//    s_send(socketGL, board_configuration);
//    s_recv(socketGL);

//    std::cout << "just sent board config to graphics thread" << std::endl;

//    // Main loop of the game
//    Board board(board_configuration, verbose);

//    std::cout << "Client process ready to play!" << std::endl;

//    std::string move_str;
//    std::string ret;
//    bool end_game = false;
//    while(!end_game)
//    {
//        board.print();

//        if (board.getCurrentPlayer() == this_player)
//        {
//            move_str = s_recv(socketGL);
//            if (board.isValidMove(move_str, this_player))
//            {
//                s_send(socketServer, move_str);
//                ret = s_recv(socketServer);
//                if (ret == move_str)
//                {
//                    // Move was validated by server
//                    board.playMove(move_str);
//                    s_send(socketGL, "accept");
//                }
//                else if (ret == "reject")
//                {
//                    s_send(socketGL, "reject");
//                }
//                else
//                {
//                    //Throw error
//                    std::cout << "ERROR: server sent '" << ret << "' back but reject or move '" << move_str << "' was expected." << std::endl;
//                    pthread_cancel(thr_GL.native_handle()); // This should do what I want: kill the thread
//                    return;
//                }
//            }
//            else
//            {
//                // Move sent by the graphics thread is not valid
//                s_send(socketGL, "reject");
//            }
//        }
//        else
//        {
//            // Other player's turn, waiting for message from server
//            move_str = s_recv(socketServer);
//            board.playMove(move_str);
//            s_send(socketGL, move_str);
//        }

//        if (board.gameFinished() != 0)
//        {
//            end_game = true;
//        }
//    }

//    // Clean up and terminate
//    thr_GL.join();

//    socketServer.close();
//    socketGL.close();
}
