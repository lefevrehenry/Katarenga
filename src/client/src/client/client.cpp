#include "client.hpp"

// Katarenga
#include <common/board/Board.hpp>
#include <common/messages/message.hpp>
#include <common/messages/messages.hpp>

// Standard Library
#include <unistd.h>
#include <string>

Client::Client(const ServerInfo& server_info, const AppInfo& app_info) :
    m_server_info(server_info),
    m_app_info(app_info),
    m_should_quit(false),
    m_poller(),
    m_connection_socket(this, context(), server_endpoint()),
    m_game(nullptr)
{
    m_poller.add(m_connection_socket, zmqpp::poller::poll_in);
    m_poller.add(STDIN_FILENO, zmqpp::poller::poll_in);
}

Client::~Client()
{
    close_server_socket();
}

zmqpp::context* Client::context() const
{
    return m_app_info.context;
}

zmqpp::endpoint_t Client::app_endpoint() const
{
    return m_app_info.thread_endpoint;
}

zmqpp::endpoint_t Client::server_endpoint() const
{
    return m_server_info.processus_endpoint;
}

void Client::poll()
{
//    msg_client("Starting main loop of the client");

//    while(!m_should_quit)
//    {
        if(m_poller.poll(0))
        {
            if(m_poller.has_input(m_connection_socket))
            {
                m_connection_socket.process_input_message();
            }

            if(m_server_socket && m_poller.has_input(*m_server_socket))
            {
                m_server_socket->process_input_message();
            }

            if(m_poller.has_input(STDIN_FILENO))
            {
                std::string command;
                std::getline(std::cin, command);

                // process the command read from std::cin
                process_command_line(command);
            }
        }
//    }

//    msg_client("client ending");

//    return 0;
}

void Client::close_connection()
{
    if(!m_game)
        return;

    m_game->set_server_socket(nullptr);

    destroy_game();
}

void Client::init_game(GameActor actor, const std::string& position, const ServerSocket::SPtr& socket)
{
    destroy_game();
    create_game(actor);

    {
        m_game->set_server_socket(socket);
        m_game->init_from_position(position, actor);
    }

    position_changed(m_game->position());
}

bool Client::play_move(Common::Move move)
{
    if(!m_game)
        return false;

    bool ok = m_game->play(move);

    if(ok)
        move_played(move);

    return ok;
}

//void Client::quit()
//{
//    m_should_quit = true;
//}

ServerSocket::SPtr Client::server_socket() const
{
    return m_server_socket;
}

void Client::open_server_socket(const zmqpp::endpoint_t& endpoint)
{
    if(m_server_socket)
        return;

    m_server_socket = std::make_shared<ServerSocket>(this, context(), endpoint);

    m_poller.add(*m_server_socket, zmqpp::poller::poll_in);
}

void Client::close_server_socket()
{
    if(!m_server_socket)
        return;

    m_poller.remove(*m_server_socket);

    m_server_socket.reset();
}

void Client::create_game(GameActor actor)
{
    m_game = std::make_shared<Game>(actor);

//    nod::signal<void(Common::Move)>& S = this->move_played;

    m_game->move_played.connect([this](const Common::Move& move) {
        this->move_played(move);
    });
}

void Client::destroy_game()
{
    m_game.reset();
}

void Client::process_command_line(const std::string& command)
{
//    msg_client("running '" + command + "'");

    if(command.empty())
    {
        // does nothing
    }
    else if(command == "h" || command == "help")
    {
        msg_client("h,help for help");
        msg_client("s,status");
        msg_client("m,move for play a move");
        msg_client("p,ping to ping the server");
        msg_client("b,board to print the board");
        msg_client("ct,create to create a party");
        msg_client("jn,join an existing party");
        msg_client("dt,board to destroy a party");
        msg_client("co,connect for start a connection with the server");
        msg_client("disco,disconnect for disconnect with the server");
        msg_client("q,quit for quit the Katarenga");
    }
    else if(command == "s" || command == "status")
    {
//        msg_client(std::string("server connected: ") + (server_connected() ? "🟢" : "🔴"));
        std::cout << "connected: " << (server_connected() ? u8"\xF0\x9F\x9F\xA2" : u8"\xF0\x9F\x94\xB4") << std::endl;
        std::cout << "endpoint: " << (server_connected() ? m_server_socket->endpoint() : "\\") << std::endl;
        if(in_game())
            m_game->print_board();
        else
            std::cout << "board: \\" << std::endl;
    }
    else if(command == "m" || command == "move")
    {
        if(m_game) {
            std::cout << "Enter your move 'from':'to' (example a2:c4): ";

            std::string str_move;
            std::cin >> str_move;

            Common::Move move = convert_to_move(str_move);

            if(is_valid_index(move)) {
                bool ok = m_game->request_play(move);
                if(!ok)
                    msg_client("cannot request move '" + str_move + "'");
            } else {
                msg_client("invalid move '" + str_move + "'");
            }

        } else {
            msg_client("No current game");
        }
    }
    else if(command == "p" || command == "ping")
    {
        m_connection_socket.request<Ping>();
    }
    else if(command == "b" || command == "board")
    {
        if(m_game) {
            m_game->print_board();
        } else {
            msg_client("No current game");
        }
    }
    else if(command == "ct" || command == "create")
    {
        if(m_server_socket) {
            m_server_socket->send_message<CreateGame>();
        } else {
            msg_client("client is not connected");
        }
    }
    else if(command == "jn" || command == "join")
    {
        if(m_server_socket) {
            m_server_socket->send_message<JoinGame>();
        } else {
            msg_client("client is not connected");
        }
    }
    else if(command == "dt" || command == "destroy")
    {
        if(m_server_socket) {
        } else {
            msg_client("client is not connected");
        }
    }
    else if(command == "co" || command == "connect")
    {
        if(!m_server_socket) {
            msg_client("sending connection request ...");
            m_connection_socket.request<NewConnection>();
        } else {
            msg_client("client is already connected");
        }
    }
    else if(command == "deco" || command == "disconnect")
    {
        if(m_server_socket) {
            msg_client("close connection ...");
            m_server_socket->send_message<CloseConnection>();
            close_server_socket();
        } else {
            msg_client("client is not connected");
        }
    }
    else if(command == "q" || command == "quit")
    {
        m_should_quit = true;
    }
    else
    {
        msg_client("unknow command '" + command + "'");
    }
}
