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

using MessageType = MessageWrapper::MessageType;

template< typename T >
void construct_reply(const MessageWrapper*, MessageWrapper*);

template<>
void construct_reply<BoardConfiguration>(const MessageWrapper*, MessageWrapper* reply)
{
    if(!server_communication)
        return;

    BoardConfiguration::Reply* object = dynamic_cast<BoardConfiguration::Reply*>(reply);

    std::string boardConfig = server_communication->getBoardConfiguration();
    object->setConfiguration(boardConfig);
}

template<>
void construct_reply<CheckConnectivity>(const MessageWrapper*, MessageWrapper* reply)
{
    if(!server_communication)
        return;

    CheckConnectivity::Reply* object = dynamic_cast<CheckConnectivity::Reply*>(reply);

    bool connectivity = server_communication->checkServerConnectivity();
    object->setConnectivity(connectivity);
}


/* function executed in its own thread */
void server_communication_function()
{
    // Get the zmq context of the Player to be able to communicate through the 'inproc' protocol
    zmqpp::context* context = PlayerInfo.context;

    int this_player = MainArguments.player;
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

    // Setup a pair socket to etablish a connection with the player socket
    zmqpp::socket socker_player(*context, zmqpp::socket_type::pair);
    socker_player.bind("inproc://" + player_binding_point);

    // Setup the poller
    // Listen to the socket_player
    zmqpp::poller poller;
    poller.add(socker_player, zmqpp::poller::poll_in);

    // Setup the message reactor
    // Associates one callback with one type of request
    MessageReactor reactor;
//    reactor.add(MessageType::AskBoardConfiguration, construct_reply<BoardConfiguration>);
//    reactor.add(MessageType::CheckConnectivity, construct_reply<CheckConnectivity>);

    bool end_game = false;

    while(!end_game)
    {
        if(poller.poll(zmqpp::poller::wait_forever))
        {
            if(poller.has_input(socker_player)) {
                // pull the request message
                zmqpp::message request_message;
                socker_player.receive(request_message);

//                // construct and push the reply message
//                zmqpp::message reply_message = reactor.process_request(request_message);
//                socker_player.send(reply_message);
            }
        }
    }
}
