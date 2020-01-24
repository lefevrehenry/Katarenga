#include "server_communication.hpp"

#include "katarenga.hpp"
#include "player.hpp"
#include "AbstractServer.hpp"
#include "LocalServer.hpp"
#include "RemoteServer.hpp"

#include <message/message_utils.hpp>

// Standard Library
#include <memory>

std::unique_ptr<AbstractServer> server_communication;

zmqpp::socket* _socker_player = nullptr;

using MessageType = MessageWrapper::MessageType;

template< typename T >
void construct_reply(zmqpp::message& request_message, zmqpp::message& reply_message)
{

}

bool process_player_thread_message(zmqpp::message& request_message)
{
    // the message returned
    zmqpp::message reply_message;

    // read the header (correspond to the type of the request sent)
    MessageType type = *request_message.get<const MessageType*>(0);

    // according to the type of the request we construct the reply message
    switch (type) {
    case MessageType::AskBoardConfiguration: {
        construct_reply<BoardConfiguration>(request_message, reply_message);
        break;
    }
    case MessageType::CheckConnectivity: {
        construct_reply<CheckConnectivity>(request_message, reply_message);
        break;
    }
    case MessageType::IsThisMoveValid: {
        break;
    }
    case MessageType::PlayThisMove: {
        break;
    }
    case MessageType::ForgetItRageQuit: {
        break;
    }
    }

    _socker_player->send(reply_message);

    return true;
}

void server_communication_function()
{
    zmqpp::context* context = PlayerInfo.context;

    int this_player = MainArguments.player;
//    int graphics_port = MainArguments.graphics_port;
    std::string server_ip = MainArguments.server_ip;
    int server_white_port = MainArguments.server_white_port;
    int server_black_port = MainArguments.server_black_port;
//    bool verbose = MainArguments.verbose;

    std::string server_port = std::to_string(this_player == 1 ? server_white_port : server_black_port);

    if (MainArguments.is_standalone)
        server_communication.reset(new LocalServer());
    else
        server_communication.reset(new RemoteServer(server_ip, server_port));

    if (!server_communication)
        throw std::runtime_error("enable to start a communication with the server");

    if (!server_communication->checkServerConnectivity())
        throw std::runtime_error("unreacheable server. Check your connectivity or be sure you enter a valid ip:port");


    std::string player_binding_point = PlayerInfo.server_communication_binding_point;
//    std::string player_binding_point = PlayerInfo.server_communication_binding_point;

    // Setup a pair socket to etablish a connection with the player socket
    zmqpp::socket socker_player(*context, zmqpp::socket_type::pair);
    socker_player.bind("inproc://" + player_binding_point);

    // Setup the poller
    zmqpp::poller poller;
    poller.add(socker_player, zmqpp::poller::poll_in);
//    poller.add(socket_render_thread, zmqpp::poller::poll_in);


    bool end_game = false;

    while(!end_game)
    {
        if(poller.poll(zmqpp::poller::wait_forever)) {

            if(poller.has_input(socker_player)) {
                // receive the message
                zmqpp::message message;
                socker_player.receive(message);

                if(!process_player_thread_message(message))
                    end_game = false;
            }
        }
    }
}
