#include "server.hpp"
#include "Board.hpp"

void Server::process_player_check_connectivity(zmqpp::message& message)
{
    CheckConnectivity o = ConstructObject<CheckConnectivity>(message);

    int player = o.getPlayer();

    if(player == 1)
    {
        // check wether the white player is already connected or not
        bool accepted = (m_connectiviy.first == false);

        // we accept the white player !
        if(accepted)
        {
            m_connectiviy.first = true;

            std::string board_configuration = m_board->getBoardConfiguration();
            int currentPlayer = m_board->getCurrentPlayer();

            // send him the game state
            zmqpp::message output_message = ConstructMessage<GameInit>(board_configuration, currentPlayer);
            m_white_player_socket.send(output_message);
        }
    }
    else if (player == -1)
    {
        // check wether the black player is already connected or not
        bool accepted = (m_connectiviy.second == false);

        // we accept the black player !
        if(accepted)
        {
            m_connectiviy.second = true;

            std::string board_configuration = m_board->getBoardConfiguration();
            int currentPlayer = m_board->getCurrentPlayer();

            // send him the game state
            zmqpp::message output_message = ConstructMessage<GameInit>(board_configuration, currentPlayer);
            m_black_player_socket.send(output_message);
        }
    }
    else
    {
        server_msg("an unknow player try to connect with the server");
    }
}

void Server::process_player_move_message(zmqpp::message& message)
{
    MoveMessage move_msg = ConstructObject<MoveMessage>(message);
    MoveType move_type = move_msg.getType();
    if (move_type == MoveType::PlayThisMove)
    {
        int move_player = move_msg.getPlayer();
        if (move_player == m_current_player)
        {
            // That's a move from the current player, handle it
            std::string move_str = move_msg.getMove();
            if (m_board->isValidMove(move_str, move_player))
            {
                // The move is accepted, update the Board state
                m_board->playMove(move_str);

                // Notify players
                move_msg.setType(MoveType::MovePlayed);
                zmqpp::message message = ConstructMessage<MoveMessage>(move_msg);
                sendToBoth(message);

                // Check if the player has won
                if (m_board->isGameFinished())
                {
                    sendWonMessage();
                }
            }
            else
            {
                // This is not a valid move, reject it
                rejectMove(move_msg);
            }
        }
        else
        {
            // That's a move from the other player, reject it
            rejectMove(move_msg);
        }
    }
    // Else, the player send a message of type MovePlayed or InvalidMove, ignore it
}

void Server::rejectMove(MoveMessage& move_msg)
{
    move_msg.setType(MoveType::InvalidMove);

    zmqpp::message m = ConstructMessage<MoveMessage>(move_msg);
    sendToPlayer(m, move_msg.getPlayer());
}

void Server::sendWonMessage()
{
    zmqpp::message message = ConstructMessage<PlayerWon>(m_board->whoWon());
    sendToBoth(message);
}


void Server::process_player_ask_board_configuration(zmqpp::message& input_message)
{
    AskBoardConfiguration m = ConstructObject<AskBoardConfiguration>(input_message);
    int player = m.getPlayer();

    zmqpp::message output_message = ConstructMessage<AnswerBoardConfiguration>(m_board->getBoardConfiguration());
    if (player == 1)
    {
        m_white_player_socket.send(output_message);
    }
    else
    {
        m_black_player_socket.send(output_message);
    }
}

void Server::process_player_stop_game(zmqpp::message& message)
{
    StopGame m = ConstructObject<StopGame>(message);
    int player = m.getPlayer();

    if (player == 1)
    {
        std::string reason = "White player: " + m.getReason();
        zmqpp::message message = ConstructMessage<GameStopped>(reason);
        m_black_player_socket.send(message);
    }
    else
    {
        std::string reason = "Black player: " + m.getReason();
        zmqpp::message message = ConstructMessage<GameStopped>(reason);
        m_white_player_socket.send(message);
    }

    // We do not treat any more message and quit
    m_game_stopped = true;
}



void Server::sendToBoth(zmqpp::message& message)
{
    m_white_player_socket.send(message);
    m_black_player_socket.send(message);
}

void Server::sendToPlayer(zmqpp::message& message, int player)
{
    if (player == 1)
    {
        m_white_player_socket.send(message);
    }
    else
    {
        m_black_player_socket.send(message);
    }
}


Server::Server(ServerInfo &server_info) :
    m_zmq_context(),
    m_poller(),
    m_white_player_socket(m_zmq_context, zmqpp::socket_type::pair),
    m_black_player_socket(m_zmq_context, zmqpp::socket_type::pair),
    m_player_reactor(),
    m_board(new Board()),
    m_current_player(),
    m_game_stopped(false),
    m_connectiviy({false,false})
{
    m_white_player_socket.bind(server_info.white_binding_point);
    m_black_player_socket.bind(server_info.black_binding_point);

    m_poller.add(m_white_player_socket, zmqpp::poller::poll_in);
    m_poller.add(m_black_player_socket, zmqpp::poller::poll_in);

    using Callback = MessageReactor::Callback;

    /* Create the function callback here and add them to the player_reactor */
    Callback process_player_check_connectivity      = std::bind(&Server::process_player_check_connectivity, this, std::placeholders::_1);
    Callback process_player_ask_board_configuration = std::bind(&Server::process_player_ask_board_configuration, this, std::placeholders::_1);
    Callback process_player_move_message            = std::bind(&Server::process_player_move_message, this, std::placeholders::_1);
    Callback process_player_stop_game               = std::bind(&Server::process_player_stop_game, this, std::placeholders::_1);

    m_player_reactor.add(MessageWrapper::MessageType::CheckConnectivity, process_player_check_connectivity);
    m_player_reactor.add(MessageWrapper::MessageType::AskBoardConfiguration, process_player_ask_board_configuration);
    m_player_reactor.add(MessageWrapper::MessageType::MoveMessage, process_player_move_message);
    m_player_reactor.add(MessageWrapper::MessageType::StopGame, process_player_stop_game);
}

Server::~Server()
{
    m_white_player_socket.close();
    m_black_player_socket.close();
}

void Server::new_game()
{
    m_board->setBoardCellTypes(generateBoardCellTypes());
    m_current_player = m_board->getCurrentPlayer();
    m_game_stopped = false;

    server_msg("New game created");
}

void Server::loop()
{
    server_msg("Starting main loop of the game");

    while(!m_board->isGameFinished() && !m_game_stopped)
    {
        zmqpp::message input_message;

        // TODO at some point we'll need to have a heartbeat of the two players
        // (or a mean to check whether they are still connected)
        // and stop the game when one player does not respond anymore
        if(m_poller.poll(zmqpp::poller::wait_forever))
        {
            // First receive the message
            if(m_poller.has_input(m_white_player_socket))
            {
                // receive the message
                m_white_player_socket.receive(input_message);
            }
            else if (m_poller.has_input(m_black_player_socket))
            {
                // receive the message
                m_black_player_socket.receive(input_message);
            }
            else
            {
                server_msg("This should not happen, terminating");
                zmqpp::message message = ConstructMessage<GameStopped>("Server is broken");
                sendToBoth(message);
                std::terminate();
            }

            // Will call the callback corresponding to the message type
            bool processed = m_player_reactor.process_message(input_message);

            if(!processed)
                server_msg("message received from on of the player socket but no callback were defined for its type");
        }
    }
}
