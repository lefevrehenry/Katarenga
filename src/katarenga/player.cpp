#include "player.hpp"
#include "katarenga.hpp"
#include "graphics.hpp"
#include "utils.hpp"
#include <message/message_utils.hpp>

#include <zmqpp/zmqpp.hpp>

//#include <iostream>
#include <string>
#include <thread>
#include <memory>

struct PlayerInfo PlayerInfo;

zmqpp::socket* _server_socket = nullptr;
zmqpp::socket* _render_socket = nullptr;

/*template< typename T >
void proccess_message_from_server(zmqpp::message&);

template<>
void proccess_message_from_server<BoardConfiguration>(zmqpp::message& input_message)
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

    bool ret = _server_socket->send(message, true);
}

void stop_game(zmqpp::message&)
{
    std::cout << "stop_game" << std::endl;
    end_game = true;
}*/

void process_server_board_configuration(zmqpp::message& message)
{
    AnswerBoardConfiguration m(message);
    //m.fromMessage(message);
    std::string configuration = m.getConfiguration();
    size_t size = configuration.size();

    // Update PlayerInfo
    if (configuration[size-1] == '+')
    {
        PlayerInfo.current_player = 1;
    }
    else if (configuration[size-1] == '-')
    {
        PlayerInfo.current_player = -1;
    }
    else
    {
        std::string err = "Wrong current player sent in BoardConfiguration message: " + configuration.at(size-1);
        player_msg(err);
        std::terminate();
    }

    // TODO at some point we'll need to update the Board informations kept by the client as well

    // Then forward it to the render thread
    _render_socket->send(message);
}

void process_server_move_message(zmqpp::message& message)
{
    MoveMessage m(message);
    //m.fromMessage(message);
    std::string move = m.getMove();
    int move_player;

    switch(m.getType())
    {
    case MoveType::MovePlayed:
        move_player = (move.at(0) == '+' ? 1 : -1);
        if (move_player != PlayerInfo.current_player)
        {
            throw std::runtime_error("Move received from a player not in his turn.");
            // TODO need to ask the correct board configuration to the server instead of throwing an error
        }
        else
        {
            PlayerInfo.current_player = -PlayerInfo.current_player;

            // TODO at some point we'll need to update the Board informations
        }
        break;
    case MoveType::InvalidMove:
        // TODO check if it's the same as the pending move
        break;
    case MoveType::PlayThisMove:
        throw std::runtime_error("PlayThisMove received from the server, this should not happen");
    }

    // Forward it to the render thread
    _render_socket->send(message);
}

void process_server_player_won(zmqpp::message& message)
{
    PlayerWon m(message);
    //m.fromMessage(message);

    PlayerInfo.game_finished = true;
    PlayerInfo.current_player = m.getPlayer();
    //player_msg((PlayerInfo.current_player == 1 ? "White" : "Black") + " player won the game!");

    // Forward it to the render thread
    _render_socket->send(message);
}

void process_server_game_stopped(zmqpp::message& message)
{
    GameStopped m(message);
    //m.fromMessage(message);

    PlayerInfo.game_finished = true;
    PlayerInfo.current_player = 0;
    //player_msg("The game stopped: " + m.getReason());

    // Forward it to the render thread
    _render_socket->send(message);
}

void player_function()
{
    PlayerInfo.self_player = MainArguments.self_player;

    zmqpp::poller poller;
    init_sockets_and_poller(poller);

    // Create the render thread
    std::thread render_thread(graphics_function);

    // Add callback functions to react to messages received from the server
    MessageReactor server_thread_reactor;
    server_thread_reactor.add(MessageWrapper::MessageType::AnswerBoardConfiguration, process_server_board_configuration);
    server_thread_reactor.add(MessageWrapper::MessageType::MoveMessage, process_server_move_message);
    server_thread_reactor.add(MessageWrapper::MessageType::PlayerWon, process_server_player_won);
    server_thread_reactor.add(MessageWrapper::MessageType::GameStopped, process_server_game_stopped);

    MessageReactor render_thread_reactor;
    // TODO see what kind of message the render thread will send
    //render_thread_reactor.add(MessageWrapper::MessageType::PrintBoard, print_board_please);
    //render_thread_reactor.add(MessageWrapper::MessageType::StopGame, stop_game);

    player_msg("Player entering the main loop!");

    zmqpp::message input_message;
    while(!PlayerInfo.game_finished)
    {
        if(poller.poll(zmqpp::poller::wait_forever))
        {
            // receive the message
            _render_socket->receive(input_message);

            if(poller.has_input(*_server_socket)) {
                // Will call the callback corresponding to the message type
                server_thread_reactor.process_message(input_message);
            }
            else if(poller.has_input(*_render_socket)) {
                // Will call the callback corresponding to the message type
                render_thread_reactor.process_message(input_message);
            }
            else
            {
                player_msg("This should not happen, terminating.");
                std::terminate(); // Forcefully terminates the execution
            }
        }
    }

    // Clean up and terminate
    render_thread.join();

    // Close the 2 sockets
    _server_socket->close();
    _render_socket->close();

    player_msg("Terminating.");
}


void init_sockets_and_poller(zmqpp::poller& poller)
{
    // Create a zmqpp context for the main thread
    zmqpp::context zmq_context;

    std::string server_ip = MainArguments.server_ip;
    std::string server_port = std::to_string(MainArguments.server_port);
    //int graphics_port = MainArguments.graphics_port;

    std::string server_binding_point = "tcp://" + server_ip + ":" + server_port;
    std::string render_binding_point = "inproc://katarenga-render-thread";

    // Give global access to the player's context
    PlayerInfo.zmq_context = &zmq_context;

    // Setup a pair socket to etablish a connection with the server
    zmqpp::socket socket_server_thread(zmq_context, zmqpp::socket_type::pair);
    socket_server_thread.bind(server_binding_point);
    _server_socket = &socket_server_thread;

    // Setup a pair socket to etablish a connection with the render thread
    zmqpp::socket socket_render_thread(zmq_context, zmqpp::socket_type::pair);
    socket_render_thread.bind(render_binding_point);
    _render_socket = &socket_render_thread;

    // We want to listen to the two sockets at the same time
    poller.add(socket_server_thread, zmqpp::poller::poll_in);
    poller.add(socket_render_thread, zmqpp::poller::poll_in);
}
