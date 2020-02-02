#include "player.hpp"

//#include "katarenga.hpp"
#include "graphics.hpp"
#include "server_communication.hpp"
//#include "AbstractServer.hpp"
//#include "LocalServer.hpp"
//#include "RemoteServer.hpp"
//#include <message/MessageReactor.hpp>
#include <message/message_utils.hpp>

#include <zmqpp/zmqpp.hpp>

//#include <iostream>
#include <thread>
#include <memory>

struct PlayerInfo PlayerInfo;

zmqpp::socket* _socket_server_communication = nullptr;
zmqpp::socket* _socket_render_thread = nullptr;

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

template< typename T >
void proccess_message_from_render_thread(zmqpp::message&);

template<>
void proccess_message_from_render_thread<BoardConfiguration>(zmqpp::message& input_message)
{
    auto call = [=](BoardConfiguration::Request*, BoardConfiguration::Reply*) {
        return std::string();
    };

    zmqpp::message reply_message = MessageReactor::ConstructReply<BoardConfiguration>(input_message, call);
}

void player_function()
{
    // Create a zmqpp context for the main thread
    zmqpp::context context;

//    int this_player = MainArguments.player;
//    int graphics_port = MainArguments.graphics_port;
//    std::string server_ip = MainArguments.server_ip;
//    int server_white_port = MainArguments.server_white_port;
//    int server_black_port = MainArguments.server_black_port;
//    bool verbose = MainArguments.verbose;

    std::string server_communication_binding_point = PlayerInfo.server_communication_binding_point;
    std::string render_binding_point = PlayerInfo.render_binding_point;

    // Give global access to the player's context
    PlayerInfo.context = &context;

    // Setup a pair socket to etablish a connection with the server_communication thread
    zmqpp::socket socket_server_communication(context, zmqpp::socket_type::pair);
    socket_server_communication.bind("inproc://" + server_communication_binding_point);
    _socket_server_communication = &socket_server_communication;

    // Setup a pair socket to etablish a connection with the render thread
    zmqpp::socket socket_render_thread(context, zmqpp::socket_type::pair);
    socket_render_thread.bind("inproc://" + render_binding_point);
    _socket_render_thread = &socket_render_thread;

    // Setup the poller, we want to listen to the two sockets at a time
    zmqpp::poller poller;
    poller.add(socket_server_communication, zmqpp::poller::poll_in);
    poller.add(socket_render_thread, zmqpp::poller::poll_in);

    // Create the 2 threads
    std::thread server_communication_thread(server_communication_function);
    std::thread render_thread(graphics_function);

    MessageReactor server_communication_reactor;
    server_communication_reactor.add(MessageWrapper::MessageType::AskBoardConfiguration, proccess_message_from_server_communication<BoardConfiguration>);

    MessageReactor render_thread_reactor;
    render_thread_reactor.add(MessageWrapper::MessageType::AskBoardConfiguration, proccess_message_from_render_thread<BoardConfiguration>);

//    std::cout << "Client process ready to play!" << std::endl;

    bool end_game = false;

    while(!end_game)
    {
        if(poller.poll(zmqpp::poller::wait_forever))
        {
            if(poller.has_input(socket_server_communication)) {
                // receive the message
                zmqpp::message input_message;
                socket_server_communication.receive(input_message);

                // proccess the message
                server_communication_reactor.process_message(input_message);

//                // forward the message
//                socket_render_thread.send(message);
            }

            if(poller.has_input(socket_render_thread)) {
                // receive the message
                zmqpp::message input_message;
                socket_render_thread.receive(input_message);

                // proccess the message
                render_thread_reactor.process_message(input_message);

//                // forward the message
//                socker_server_communication.send(message);
            }
        }
    }

    // Clean up and terminate
    server_communication_thread.join();
    render_thread.join();

    // Close the 2 sockets
    socket_server_communication.close();
    socket_render_thread.close();
}
