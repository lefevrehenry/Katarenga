#include "common_utils.hpp"

// Katarenga
#include <common/board/Board.hpp>
#include <common/messages/message.hpp>
#include <common/messages/messages.hpp>

// Ini
#include <ini/ini.h>

// Standard Library
#include <filesystem>
#include <iostream>
#include <map>
#include <string>

namespace fs = std::filesystem;

ServerInfo ReadConfigFile()
{
    std::string home = std::getenv("HOME");

    fs::path config_file = fs::path(home) / ".config" / "katarenga" / "server.cfg";

    inih::INIReader ini(config_file);

    // Get and parse the ini value
    std::string thread_port = ini.Get<std::string>("thread", "port");
    std::string processus_port = ini.Get<std::string>("processus", "port");
    std::string tcp_port = ini.Get<std::string>("tcp", "port");

    ServerInfo config;
    config.thread_endpoint = "inproc://" + thread_port;
    config.processus_endpoint = "ipc://" + processus_port;
    config.network_endpoint = "tcp://*:" + tcp_port;

    return config;
}

void Initialize()
{
    Message::Id<Ping>();
    Message::Id<typename Ping::Request>();
    Message::Id<typename Ping::Reply>();

    Message::Id<NewConnection>();
    Message::Id<typename NewConnection::Request>();
    Message::Id<typename NewConnection::Reply>();

    Message::Id<CloseConnection>();

    Message::Id<CreateGame>();
    Message::Id<JoinGame>();
    Message::Id<SpectateGame>();

    Message::Id<GameCreated>();
    Message::Id<GameJoined>();
    Message::Id<GameSpectated>();

    Message::Id<PlayMove>();
    Message::Id<MovePlayed>();
}

bool is_valid_move(const Common::Move& move)
{
    int from_line = std::get<0>(move);
    int from_row = std::get<1>(move);

    int to_line = std::get<2>(move);
    int to_row = std::get<3>(move);

    if(from_line == to_line && from_row == to_row)
        return false;

    return (from_line >= 0 && from_line <= 7 &&
            from_row >= 0 && from_row <= 7 &&
            to_line >= 0 && to_line <= 7 &&
            to_row >= 0 && to_row <= 7);
}
