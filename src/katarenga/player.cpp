#include "player.hpp"
#include "graphics.hpp"

#include <message/message_utils.hpp>

#include <functional>
#include <algorithm>

#include <iostream> // FOR DEBUGGING PURPOSE, TODO REMOVE IT

//void Player::process_server_reply_connectivity(zmqpp::message& message)
//{
//    ReplyConnectivity m = ConstructObject<ReplyConnectivity>(message);

//    bool accepted = m.getAccepted();

//    if(accepted)
//        player_msg("Connection accepted with the server");
//    else
//        player_msg("Connection rejected with the server");

//    m_connected = accepted;
//}

void Player::process_server_game_init(zmqpp::message& message)
{
    GameInit m = ConstructObject<GameInit>(message);

    // Update internal state
    m_current_player = m.getCurrentPlayer();
    retrieve_piece_locations(m.getConfiguration());
    m_connected = true;

    // Then forward to the render thread
    m_render_thread_socket.send(message);
}


void Player::process_server_board_configuration(zmqpp::message& message)
{
    AnswerBoardConfiguration m = ConstructObject<AnswerBoardConfiguration>(message);
    std::string configuration = m.getConfiguration();
    size_t size = configuration.size();

    retrieve_piece_locations(configuration);

    // Retrieve current player
    if (configuration[size-2] == '+')
    {
        m_current_player = 1;
    }
    else if (configuration[size-2] == '-')
    {
        m_current_player = -1;
    }
    else
    {
        std::string err = "Wrong current player sent in BoardConfiguration message: " + std::string(1, configuration.at(size-2));
        player_msg(err);
        std::terminate();
    }

    // Then forward it to the render thread
    m_render_thread_socket.send(message);
}

void Player::process_server_move_message(zmqpp::message& message)
{
    MoveMessage m = ConstructObject<MoveMessage>(message);
    int move_player = m.getPlayer();
    MoveType type = m.getType();

    if (type == MoveType::MovePlayed)
    {
        if (move_player != m_current_player)
        {
            throw std::runtime_error("Move received from a player not in his turn.");
            // TODO need to ask the correct board configuration to the server instead of throwing an error
        }
        else
        {
            // Update the new location of the piece if it was my move
            if (move_player == m_self_player)
            {
                int src, dest;
                convert_move_str(m.getMove(), src, dest);
                auto it = std::find(m_piece_locations.begin(), m_piece_locations.end(), src);
                if (it != m_piece_locations.end())
                {
                    *it = dest;
                    player_msg("Updating piece location from " + std::to_string(src) + " to " + std::to_string(dest));
                }
                else
                {
                    throw std::runtime_error("Bad internal state: Did not find piece location for move received");
                    // TODO need to ask the correct board configuration to the server instead of throwing an error
                }
            }
            m_current_player = -m_current_player;
        }
    }
    else if (type == MoveType::InvalidMove)
    {
        // TODO maybe do something here before forwarding it to the render thread
    }
    else
    {
        throw std::runtime_error("PlayThisMove received from the server, this should not happen");
    }

    // Forward it to the render thread
    m_render_thread_socket.send(message);
}

void Player::process_server_player_won(zmqpp::message& message)
{
    PlayerWon m = ConstructObject<PlayerWon>(message);

    m_game_finished = true;
    //m_current_player = m.getPlayer(); //TODO ??

    // Forward it to the render thread
    m_render_thread_socket.send(message);
}

void Player::process_server_game_stopped(zmqpp::message& message)
{
    //GameStopped m = ConstructObject<GameStopped>(message);

    m_game_finished = true;
    //m_current_player = 0; // TODO ??

    // Forward it to the render thread
    m_render_thread_socket.send(message);
}

void Player::retrieve_piece_locations(const std::string& board_configuration)
{
    m_piece_locations.clear();

    // Retrieve pieces from the regular board but not Camp cells
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            int cell_index = (i * 8 ) + j;
            if (board_configuration.at(2 * cell_index + 1) == m_self_player_sign)
            {
                // This cell is occupied by one of my pieces
                m_piece_locations.push_back(cell_index);
                player_msg("Found a piece at location " + std::to_string(cell_index));
            }
        }
    }
}


void Player::process_graphics_case_clicked(zmqpp::message& message)
{
    CaseClicked c = ConstructObject<CaseClicked>(message);

    int id = stoi(c.getCase());

    bool state0 = (m_memo.first == -1 && m_memo.second == -1);
    bool state1 = (m_memo.first != -1 && m_memo.second == -1);

    bool is_case_owned_by_player = std::find(m_piece_locations.begin(), m_piece_locations.end(), id) != m_piece_locations.end();

    if(state0)
    {
        if(is_case_owned_by_player)
            m_memo.first = id;
    }
    else if(state1)
    {
        if(is_case_owned_by_player)
            m_memo.first = id;
        else
            m_memo.second = id;
    }

    bool state2 = (m_memo.first != -1 && m_memo.second != -1);

    if(state2)
    {
        // envoie le coup au serveur
        std::string move_str = create_move_str(m_memo.first, m_memo.second);
        zmqpp::message play_message = ConstructMessage<MoveMessage>(MoveType::PlayThisMove, move_str, m_self_player);

        m_server_thread_socket.send(play_message, true);

        // re-init
        m_memo.first = -1;
        m_memo.second = -1;
    }

    player_msg("case clicked " + std::to_string(id));
}

void Player::process_graphics_stop_game(zmqpp::message&)
{
    m_game_finished = true;

    zmqpp::message message = ConstructMessage<StopGame>("Game stopped by the graphical interface", m_self_player);
    m_server_thread_socket.send(message, true);
}



Player::Player(GameSettings& game_settings) :
    m_zmq_context(),
    m_poller(),
    m_server_thread_socket(m_zmq_context, zmqpp::socket_type::pair),
    m_render_thread_socket(m_zmq_context, zmqpp::socket_type::pair),
    m_server_thread_reactor(),
    m_render_thread_reactor(),
    m_game_finished(false),
    m_game_stopped(false),
    m_self_player(game_settings.self_player),
    m_current_player(0),
    m_connected(false),
    m_piece_locations(),
    m_memo({-1,-1})
{
    m_server_thread_socket.connect(game_settings.server_binding_point);
    m_render_thread_socket.bind(game_settings.render_binding_point);

    // Create the render thread and
    m_render_thread = std::thread(graphics_function,
                                  std::ref(m_zmq_context),
                                  game_settings.render_binding_point);


    // We want to listen to the two sockets at the same time
    m_poller.add(m_server_thread_socket, zmqpp::poller::poll_in);
    m_poller.add(m_render_thread_socket, zmqpp::poller::poll_in);

    using MessageType = MessageWrapper::MessageType;
    using Callback = MessageReactor::Callback;

    // Add callback functions to react to messages received from the server
    Callback process_server_game_init           = std::bind(&Player::process_server_game_init, this, std::placeholders::_1);
    Callback process_server_board_configuration = std::bind(&Player::process_server_board_configuration, this, std::placeholders::_1);
    Callback process_server_move_message        = std::bind(&Player::process_server_move_message, this, std::placeholders::_1);
    Callback process_server_player_won          = std::bind(&Player::process_server_player_won, this, std::placeholders::_1);
    Callback process_server_game_stopped        = std::bind(&Player::process_server_game_stopped, this, std::placeholders::_1);

    m_server_thread_reactor.add(MessageType::GameInit, process_server_game_init);
    m_server_thread_reactor.add(MessageType::AnswerBoardConfiguration, process_server_board_configuration);
    m_server_thread_reactor.add(MessageType::MoveMessage, process_server_move_message);
    m_server_thread_reactor.add(MessageType::PlayerWon, process_server_player_won);
    m_server_thread_reactor.add(MessageType::GameStopped, process_server_game_stopped);

    // Add callback functions to react to messages received from the render thread
    Callback process_graphics_case_clicked = std::bind(&Player::process_graphics_case_clicked, this, std::placeholders::_1);
    Callback process_graphics_stop_game = std::bind(&Player::process_graphics_stop_game, this, std::placeholders::_1);

    m_render_thread_reactor.add(MessageType::CaseClicked, process_graphics_case_clicked);
    m_render_thread_reactor.add(MessageType::StopGame, process_graphics_stop_game);


    m_piece_locations.reserve(8); // Reserve space for 8 pieces
    m_self_player_sign = (m_self_player == 1) ? '+' : '-';
}

Player::~Player()
{
    m_server_thread_socket.close();
    m_render_thread_socket.close();
}

void Player::connect()
{
    if(m_connected)
        return;

    zmqpp::poller poller;   // safer to use a local poller/reactor I think
    MessageReactor reactor;

    poller.add(m_server_thread_socket, zmqpp::poller::poll_in);

    using MessageType = MessageWrapper::MessageType;
    using Callback = MessageReactor::Callback;

    Callback process_server_game_init = std::bind(&Player::process_server_game_init, this, std::placeholders::_1);
    reactor.add(MessageType::GameInit, process_server_game_init);

    zmqpp::message message = ConstructMessage<CheckConnectivity>(m_self_player);
    m_server_thread_socket.send(message, true);

    // wait 5 seconds the response of the server
    if(poller.poll(5000))
    {
        if(poller.has_input(m_server_thread_socket))
        {
            zmqpp::message input_message;

            // receive the message
            m_server_thread_socket.receive(input_message);

            // process it
            bool processed = reactor.process_message(input_message);

            if(!processed)
                player_msg("message received from the server thread but no callback were defined for its type");
        }
    }

    if(!m_connected)
        player_msg("Server not responding :(");
}

void Player::loop()
{
    while(!m_connected)
    {
        connect();
    }

    player_msg("Player entering the main loop!");

    while(!m_game_finished)
    {
        zmqpp::message input_message;

        if(m_poller.poll(zmqpp::poller::wait_forever))
        {
            if(m_poller.has_input(m_server_thread_socket))
            {
                // receive the message
                m_server_thread_socket.receive(input_message);

                // Will call the callback corresponding to the message type
                bool processed = m_server_thread_reactor.process_message(input_message);

                if(!processed)
                    player_msg("message received from the server thread but no callback were defined for its type");
            }
            else if(m_poller.has_input(m_render_thread_socket))
            {
                // receive the message
                m_render_thread_socket.receive(input_message);

                // Will call the callback corresponding to the message type
                bool processed = m_render_thread_reactor.process_message(input_message);

                if(!processed)
                    player_msg("message received from the render thread but no callback were defined for its type");
            }
            else
            {
                player_msg("This should not happen, terminating.");
                std::terminate(); // Forcefully terminates the execution
            }
        }
    }

    // Clean up and terminate
    m_render_thread.join();

    player_msg("Terminating.");
}
