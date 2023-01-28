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
}
