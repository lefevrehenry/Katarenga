#include "player.hpp"

#include "katarenga.hpp"
#include "graphics.hpp"
//#include "server_communication.hpp"
//#include "AbstractServer.hpp"
//#include "LocalServer.hpp"
//#include "RemoteServer.hpp"
//#include <message/MessageReactor.hpp>
#include <message/message_utils.hpp>

#include <zmqpp/zmqpp.hpp>

#include <iostream>
#include <string>
#include <thread>
#include <memory>

struct PlayerInfo PlayerInfo;

zmqpp::socket* _socket_server_thread = nullptr;
zmqpp::socket* _socket_render_thread = nullptr;
bool end_game = false;

template< typename T >
void proccess_message_from_server_communication(zmqpp::message&);

template<>
void proccess_message_from_server_communication<BoardConfiguration>(zmqpp::message& input_message)
{
    auto call = [=](BoardConfiguration::Request*, BoardConfiguration::Reply*) {
        return std::string();
    };

    zmqpp::message reply_message = MessageReactor::ConstructReply<BoardConfiguration>(input_message, call);
}

void print_board_please(zmqpp::message&)
{
    // ask board string to server
    std::cout << "print_board" << std::endl;
    zmqpp::message message;

    bool ret = _socket_server_thread->send(message, true);
}

void stop_game(zmqpp::message&)
{
    std::cout << "stop_game" << std::endl;
    end_game = true;
}

void player_function()
{
    // Create a zmqpp context for the main thread
    zmqpp::context context;

    bool is_standalone = MainArguments.is_standalone;
    int this_player = MainArguments.player;
//    int graphics_port = MainArguments.graphics_port;
//    std::string server_ip = MainArguments.server_ip;
    int server_white_port = MainArguments.server_white_port;
    int server_black_port = MainArguments.server_black_port;
//    bool verbose = MainArguments.verbose;

    std::string server_scheme;
    std::string server_ip;
    std::string server_port;

    if (is_standalone) {
        server_scheme = "inproc://";
        server_ip = PlayerInfo.server_binding_point;
        server_port = "";
    } else {
        server_scheme = "tcp://";
        server_ip = MainArguments.server_ip;
        server_port = std::to_string(this_player == 1 ? server_white_port : server_black_port);
    }

    std::string server_binding_point = server_scheme + server_ip + server_port;
    std::string render_binding_point = PlayerInfo.render_binding_point;

    // Give global access to the player's context
    PlayerInfo.context = &context;

    // Setup a pair socket to etablish a connection with the server_communication thread
    zmqpp::socket socket_server_thread(context, zmqpp::socket_type::pair);
    socket_server_thread.bind("inproc://" + server_binding_point);
    _socket_server_thread = &socket_server_thread;

    // Setup a pair socket to etablish a connection with the render thread
    zmqpp::socket socket_render_thread(context, zmqpp::socket_type::pair);
    socket_render_thread.bind("inproc://" + render_binding_point);
    _socket_render_thread = &socket_render_thread;

    // Setup the poller, we want to listen to the two sockets at the same time
    zmqpp::poller poller;
    poller.add(socket_server_thread, zmqpp::poller::poll_in);
    poller.add(socket_render_thread, zmqpp::poller::poll_in);

    // Create the render thread
    std::thread render_thread(graphics_function);

    MessageReactor server_thread_reactor;
    server_thread_reactor.add(MessageWrapper::MessageType::AskBoardConfiguration, proccess_message_from_server_communication<BoardConfiguration>);

    MessageReactor render_thread_reactor;
    render_thread_reactor.add(MessageWrapper::MessageType::PrintBoard, print_board_please);
    render_thread_reactor.add(MessageWrapper::MessageType::StopGame, stop_game);

//    std::cout << "Client process ready to play!" << std::endl;

    while(!end_game)
    {
        if(poller.poll(zmqpp::poller::wait_forever))
        {
            if(poller.has_input(socket_server_thread)) {
                // receive the message
                zmqpp::message input_message;
                socket_server_thread.receive(input_message);

                // proccess the message
                server_thread_reactor.process_message(input_message);
            }

            if(poller.has_input(socket_render_thread)) {
                // receive the message
                zmqpp::message input_message;
                socket_render_thread.receive(input_message);

                // proccess the message
                render_thread_reactor.process_message(input_message);
            }
        }
    }

    // Clean up and terminate
    render_thread.join();

    // Close the 2 sockets
    socket_server_thread.close();
    socket_render_thread.close();

    std::cout << "Terminating main thread." << std::endl;
}
