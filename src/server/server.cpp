#include "server.hpp"
#include "Board.hpp"

// Standard Library
#include <string>
#include <unistd.h>

std::string format_board(const std::string& board_configuration)
{
    std::string s = "";

    s += "\n";
    s += "    1  2  3  4  5  6  7  8   ";
    s += "\n";
    s += "             White           ";
    s += "\n";
    s += "  X                         X";
    s += "\n";

    std::string board = board_configuration.substr(0,128);

    for (int j = 0; j < 8; ++j) {
        s += std::to_string(j+1) + "  ";
        for (int i = 0; i < 8; ++i) {
            size_t index = (j * 8) + i;
            std::string c = board.substr(2*index,2);
            s += " " + c;
        }
        s += "\n";
    }

    s += "  X                         X";
    s += "\n";
    s += "             Black           ";

    return s;
}


void Server::process_player_check_connectivity(zmqpp::message& message)
{
    CheckConnectivity o = ConstructObject<CheckConnectivity>(message);

    int player = o.getPlayer();

    if(player == 1)
    {
        // check wether the white player is already connected or not
        if(m_connectiviy.first == false)
        {
            // we accept the white player!
            m_connectiviy.first = true;
            sendGameInit(player);
        }
    }
    else if (player == -1)
    {
        // check wether the black player is already connected or not
        if(m_connectiviy.second == false)
        {
            // we accept the black player!
            m_connectiviy.second = true;
            sendGameInit(player);
        }
    }
    else
    {
        server_msg("an unknown player tries to connect with the server");
    }
}

void Server::process_player_move_message(zmqpp::message& message)
{
    MoveMessage move_msg = ConstructObject<MoveMessage>(message);
    MoveType move_type = move_msg.getType();
    if (move_type == MoveType::PlayThisMove)
    {
        int move_player = move_msg.getPlayer();
        if (move_player == m_board->getCurrentPlayer())
        {
            // That's a move from the current player, handle it
            int src = move_msg.getSource();
            int dest = move_msg.getDestination();

            bool valid_move = m_board->playMove(src, dest); // Returns false if invalid move

            if (valid_move)
            {
                server_msg("player " + std::to_string(move_player) + " played move " + std::to_string(src) + ":" + std::to_string(dest));

                // The move was accepted, notify players
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
                server_msg("player " + std::to_string(move_player) + " cannot play move " + std::to_string(src) + ":" + std::to_string(dest));

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
    // Else, the player sent a message of type MovePlayed or InvalidMove, ignore it
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
        m_black_player_socket.send(message, true);

        server_msg(reason);
        m_connectiviy.first = false;
    }
    else
    {
        std::string reason = "Black player: " + m.getReason();
        zmqpp::message message = ConstructMessage<GameStopped>(reason);
        m_white_player_socket.send(message, true);

        server_msg(reason);
        m_connectiviy.second = false;
    }

    // We do not treat any more message and quit
    //m_game_stopped = true;
}


void Server::stop_the_game()
{
    zmqpp::message message = ConstructMessage<StopGame>("Game is closed by the server", 0);
    sendToBoth(message);

    m_game_stopped = true;

    server_msg("Game closed");
}

void Server::process_command_line(const std::string& command)
{
    if(command.size() == 0)
        return;

    // all available options here
    static std::list<std::string> options = {"help", "h", "print", "p", "move", "m", "close", "c"};

    std::string option_found = "";

    // try to find the option matching the command
    for (const std::string& option : options)
    {
        size_t option_size = option.size();

        if(command.size() >= option_size && command.substr(0, option_size) == option) {
            option_found = option;
            break;
        }
    }

    if(option_found.size() == 0) {
        server_msg("Unknow command '" + command + "'");
        return;
    }

    if(option_found == "h" || option_found == "help")
    {
        server_msg("h,help for help");
        server_msg("p,print to print the board");
        server_msg("move, play a move (format like 'move <player>:src:dst')");
        server_msg("c,close to close the server");
    }
    else if(option_found == "p" || option_found == "print")
    {
        std::string board_configuration = format_board(m_board->getBoardConfiguration());
        server_msg(board_configuration);
    }
    else if(option_found == "m" || option_found == "move")
    {
        int player = 0;
        int src_index = -1;
        int dst_index = -1;

        try {
            std::string token;

            std::string str = command;
            str.erase(0, option_found.size() + 1);

            token = str.substr(0, str.find(":"));
            str.erase(0, token.size() + 1);

            player = std::stoi(token);

            token = str.substr(0, str.find(":"));
            str.erase(0, token.size() + 1);

            src_index = std::stoi(token);
            dst_index = std::stoi(str);
        }
        catch(...)
        {
            server_msg("cannot parse move command '" + command + "'");
            return;
        }

        // emulate a move received from the player
        zmqpp::message message = ConstructMessage<MoveMessage>(MoveType::PlayThisMove, src_index, dst_index, player);
        process_player_move_message(message);
    }
    else if(option_found == "c" || option_found == "close")
    {
        stop_the_game();
    }
    else
    {
        server_msg("Unknow command '" + command + "'");
    }
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

void Server::sendGameInit(int player)
{
    zmqpp::message zmq_message = ConstructMessage<GameInit>(m_board->getBoardConfiguration(),
                                                            m_board->getCurrentPlayer());
    sendToPlayer(zmq_message, player);
}

void Server::sendToBoth(zmqpp::message& message)
{
    zmqpp::message white_message = message.copy();
    m_white_player_socket.send(white_message, true);

    zmqpp::message black_message = message.copy();
    m_black_player_socket.send(black_message, true);
}

void Server::sendToPlayer(zmqpp::message& message, int player)
{
    if (player == 1)
    {
        m_white_player_socket.send(message, true);
    }
    else
    {
        m_black_player_socket.send(message, true);
    }
}


Server::Server(ServerInfo &server_info) :
    m_zmq_context(),
    m_poller(),
    m_white_player_socket(m_zmq_context, zmqpp::socket_type::pair),
    m_black_player_socket(m_zmq_context, zmqpp::socket_type::pair),
    m_player_reactor(),
    m_board(new Board()),
    m_game_stopped(false),
    m_connectiviy({false,false})
{
    m_white_player_socket.bind(server_info.white_binding_point);
    m_black_player_socket.bind(server_info.black_binding_point);

    // Listen to both players and std::cin
    m_poller.add(m_white_player_socket, zmqpp::poller::poll_in);
    m_poller.add(m_black_player_socket, zmqpp::poller::poll_in);
    m_poller.add(STDIN_FILENO, zmqpp::poller::poll_in);

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
    m_game_stopped = false;

    server_msg("New game created");
}

void Server::loop()
{
    server_msg("Starting main loop of the game");

    while(!m_board->isGameFinished() && !m_game_stopped)
    {
        // TODO at some point we'll need to have a heartbeat of the two players
        // (or a mean to check whether they are still connected)
        // and stop the game when one player does not respond anymore
        if(m_poller.poll(zmqpp::poller::wait_forever))
        {
            if(m_poller.has_input(m_white_player_socket))
            {
                zmqpp::message input_message;

                // receive the message
                m_white_player_socket.receive(input_message);

                // Will call the callback corresponding to the message type
                bool processed = m_player_reactor.process_message(input_message);

                if(!processed)
                    server_msg("Message received from one of the player socket but no callback were defined to handle it");
            }
            else if(m_poller.has_input(m_black_player_socket))
            {
                zmqpp::message input_message;

                // receive the message
                m_black_player_socket.receive(input_message);

                // Will call the callback corresponding to the message type
                bool processed = m_player_reactor.process_message(input_message);

                if(!processed)
                    server_msg("Message received from one of the player socket but no callback were defined to handle it");
            }
            else if(m_poller.has_input(STDIN_FILENO))
            {
                std::string command;
                std::getline(std::cin, command);

                // process the command read from std::cin
                process_command_line(command);
            }
        }
    }
}
