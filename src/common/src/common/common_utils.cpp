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
#include <sstream>
#include <vector>
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

static std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> result;

   std::istringstream tokenStream(s);
   std::string token;

   while (std::getline(tokenStream, token, delimiter))
      result.push_back(token);

   return result;
}

Common::Move convert_to_move(const std::string& str_move)
{
    if(str_move.length() != 5)
        return {-1,-1};

    std::vector<std::string> splitted = split(str_move, ':');

    if(splitted.size() != 2)
        return {-1,-1};

    const std::string& from_cell = splitted[0];
    const std::string& to_cell = splitted[1];

    int from_col = int(from_cell[0]) - int('a');
    int from_row = int(from_cell[1]) - int('1');

    int to_col = int(to_cell[0]) - int('a');
    int to_row = int(to_cell[1]) - int('1');

    int from = (from_row * 8) + from_col;
    int to = (to_row * 8) + to_col;

    return {from, to};
}

bool is_valid_index(const Common::Move& move)
{
    int from = std::get<0>(move);
    int to = std::get<1>(move);

    if(from == to)
        return false;

//    int from_row = from / 8;
//    int from_col = from % 8;

//    int to_row = to / 8;
//    int to_col = to % 8;

    bool from_valid = (from >= 0 && from < 64);
    bool to_valid = ((from >= 0 && from < 64) || to == -8 || to == -1 || to == 64 || to == 71);

    return from_valid && to_valid;
}
