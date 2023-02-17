#include "server.hpp"

// Katarenga
#include <common/board/Board.hpp>
#include <common/board/board_utils.hpp>

// Standard Library
#include <string>
#include <unistd.h>

Server::Server(const ServerInfo& server_info) :
    m_server_info(server_info),
    m_should_quit(false),
    m_poller(),
    m_connection_socket(this, context(), endpoint()),
    m_client_sockets(),
    m_client_registry(),
    m_game_registry()
{
    m_poller.add(m_connection_socket, zmqpp::poller::poll_in);
    m_poller.add(STDIN_FILENO, zmqpp::poller::poll_in);
}

zmqpp::context* Server::context() const
{
    return m_server_info.context;
}

zmqpp::endpoint_t Server::endpoint() const
{
    return m_server_info.processus_endpoint;
}

void Server::loop()
{
    msg_server("Starting main loop of the server");

    while(!m_should_quit)
    {
//        msg_server("poll");
        std::cout << ">>> " << std::flush;
        if(m_poller.poll(zmqpp::poller::wait_forever))
        {
            if(m_poller.has_input(m_connection_socket))
            {
                std::cout << std::endl;

//                msg_server("new connection request");
                m_connection_socket.process_input_message();
            }

            for (const PlayerSocket::SPtr& socket : m_client_sockets) {
                if(m_poller.has_input(*socket))
                {
                    std::cout << std::endl;

//                    msg_server("message received");
                    socket->process_input_message();
                }
            }

            if(m_poller.has_input(STDIN_FILENO))
            {
                std::string command;
                std::getline(std::cin, command);

                // process the command read from std::cin
                process_command_line(command);
            }
        }
    }

    msg_server("Exiting main loop of the server");
}

static zmqpp::endpoint_t create_new_client_endpoint(const zmqpp::endpoint_t& endpoint)
{
    static int N = 1;

    zmqpp::endpoint_t client_endpoint = endpoint + std::to_string(N++);

    return client_endpoint;
}

zmqpp::endpoint_t Server::new_connection(const std::string& ip, const std::string& port)
{
    ClientRegistry::ClientId id = ClientRegistry::Id(ip, port);

    ClientRegistry* registry = &m_client_registry;
    msg_server("Client '" + id + "' request new connection");

    if(registry->client_exists(id)) {
        ClientRegistry::ClientSocket::SPtr socket = registry->socket(id);
        msg_server("Client '" + id + "' already exists in the database. Listen to him at '" + socket->endpoint() + "'");

        return socket->endpoint();
    }

    ClientSocket::SPtr socket = std::make_shared<ClientSocket>(this, context(), create_new_client_endpoint(endpoint()));

    if(!registry->add_client(id, socket))
        return "";

    msg_server("Client '" + id + "' create new socket connection. Listen to him at '" + socket->endpoint() + "'");
    start_monitor_socket(socket);

    return socket->endpoint();
}

void Server::close_connection(const ClientSocket::SPtr& socket)
{
    ClientRegistry* registry = &m_client_registry;

    ClientId id = registry->id(socket);

//    if(id.empty()) {
//        msg_server("ClientSocket id not registered");
//        return;
//    }

    msg_server("Client '" + id + "' close connection with server");
    stop_monitor_socket(socket);

    registry->remove_client(id);
}

Server::GameId Server::create_game(GameActor actor, const ClientSocket::SPtr& socket)
{
    GameRegistry* registry = &m_game_registry;

    // the socket is not already busy by a another Game ?
    if(!socket->busy())
    {
        GameRegistry::GameId id = Game::GenerateId();
        Game::SPtr game = Game::Create();

        switch (actor) {
        case GameActor::White:
            game->set_white_socket(socket);
            break;
        case GameActor::Black:
            game->set_black_socket(socket);
            break;
        default:
            game->set_white_socket(socket);
        }

        registry->add_game(id, game);

        return id;
    }

    return GameId();
}

Server::GameId Server::join_game(GameId id, const ClientSocket::SPtr& socket)
{
    if(socket->busy() && id == GameId())
        id = socket->id();

    const GameRegistry* registry = &m_game_registry;

    Game::SPtr game = registry->find_game(id);

    // game exists obviously ?
    if(game)
    {
        GameId _id = GameId();

        // if player is already in the game
        if(game->is_white_player(socket)) {
            _id = id;
        } else if(game->is_black_player(socket)) {
            _id = id;
        }
        // otherwise is game waiting for players ?
        else if(game->is_pending())
        {
            // the socket is not already busy by a game ?
            if(!socket->busy())
            {
                if(game->has_white_player()) {
                    game->set_black_socket(socket);
                    _id = id;
                } else if(game->has_black_player()) {
                    game->set_white_socket(socket);
                    _id = id;
                }
            }
        }

        return _id;
    }

    return GameId();
}

Server::GameId Server::spectate_game(GameId id, const PlayerSocket::SPtr& socket)
{
    const GameRegistry* registry = &m_game_registry;

    Game::SPtr game = registry->find_game(id);

    // game exists obviously ?
    if(game)
    {
        // TODO: do it
        return id;
    }

    return GameId();
}

Common::Position Server::game_position(GameId id) const
{
    const GameRegistry* registry = &m_game_registry;

    Game::SPtr game = registry->find_game(id);

    return (game ? game->position() : "");
}

Server::GameActor Server::game_actor(GameId id, const PlayerSocket::SPtr& socket) const
{
    const GameRegistry* registry = &m_game_registry;

    Game::SPtr game = registry->find_game(id);

    return (game ? game->actor(socket) : GameActor::None);
}

bool Server::play_move(GameId id, Move move, GameActor actor)
{
    const GameRegistry* registry = &m_game_registry;

    Game::SPtr game = registry->find_game(id);

    // game exists obviously ?
    if(!game)
        return false;

    // confirm the move has been played (or not)
    return game->play(move, actor);
}

void Server::start_monitor_socket(const ClientSocket::SPtr& socket)
{
    m_client_sockets.push_back(socket);
    m_poller.add(*socket, zmqpp::poller::poll_in);
}

void Server::stop_monitor_socket(const ClientSocket::SPtr& socket)
{
    auto it = std::find(m_client_sockets.begin(), m_client_sockets.end(), socket);

    if(it == m_client_sockets.end())
        return;

    m_client_sockets.erase(it);
    m_poller.remove(*socket);
}

void Server::process_command_line(const std::string& command)
{
//    msg_server("running '" + command + "'");

    if(command.empty())
    {
        // does nothing
    }
    else if(command == "h" || command == "help")
    {
        msg_server("h,help for help");
        msg_server("s,status");
        msg_server("b,board");
        msg_server("q,quit for quit the server");
    }
    else if(command == "s" || command == "status")
    {
        msg_server(std::to_string(m_client_sockets.size()) + " clients connected");
    }
    else if(command == "b" || command == "board")
    {
        ServerCommon::GameId id = 1;
        Game::SPtr game = m_game_registry.m_games.count(id) ? m_game_registry.m_games.at(id) : nullptr;
        if(game) {
            game->print_board();
        } else {
            msg_server("No curent game");
        }

    }
    else if(command == "q" || command == "quit")
    {
        m_should_quit = true;
    }
    else
    {
        msg_server("unknow command '" + command + "'");
    }
}
