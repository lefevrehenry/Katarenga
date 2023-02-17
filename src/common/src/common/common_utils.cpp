#include "common_utils.hpp"

// Katarenga
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

template< typename M >
static void registerMessage()
{
    std::cout << Message::Id<M>() << " -> " << Message::Name<M>() << std::endl;
}

void Initialize()
{
    registerMessage<Ping>();
    registerMessage<typename Ping::Request>();
    registerMessage<typename Ping::Reply>();

    registerMessage<NewConnection>();
    registerMessage<typename NewConnection::Request>();
    registerMessage<typename NewConnection::Reply>();

    registerMessage<CloseConnection>();

    registerMessage<CreateGame>();
    registerMessage<JoinGame>();
    registerMessage<SpectateGame>();

    registerMessage<GameCreated>();
    registerMessage<GameJoined>();
    registerMessage<GameSpectated>();

    registerMessage<typename PlayMove::Request>();
    registerMessage<typename PlayMove::Reply>();
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
    Common::Case from = move_from(move);
    Common::Case to = move_to(move);

    if(from == to)
        return false;

    bool from_valid = (from >= 0 && from < 64);
    bool to_valid = ((from >= 0 && from < 64) || to == -8 || to == -1 || to == 64 || to == 71);

    return from_valid && to_valid;
}

static int plateau_begin_offset = 0;
static int plateau_end_offset = plateau_begin_offset + 136;

static int camp_cells_begin_offset = plateau_end_offset;
static int camp_cells_end_offset = camp_cells_begin_offset + 4;

static int current_player_begin_offset = camp_cells_end_offset;
static int current_player_end_offset = current_player_begin_offset + 1;

static int winning_player_begin_offset = current_player_end_offset;
static int winning_player_end_offset = winning_player_begin_offset + 1;

bool has_valid_format(const Common::Position& position)
{
    return ((position.begin() + winning_player_end_offset) == position.end());
}

void yield_cases(const Common::Position& position, std::function<void(Common::Case, Common::CellType, Common::GameActor)> callback)
{
    using iter = Common::Position::const_iterator;

    iter plateau_begin = position.begin() + plateau_begin_offset;
    iter plateau_end = position.begin() + plateau_end_offset;

    int index = 0;

    for (iter it = plateau_begin; it != plateau_end; ++it)
    {
        if(*it == '\n')
            continue;

        Common::Case c = (index++);

        char ch = *it;
        Common::CellType type = to_cell_type(ch);

        if(type == Common::CellType::None)
            throw std::runtime_error("unknown character '" + std::to_string(ch) + "'");

        ch = *(++it);
        Common::GameActor actor = to_game_actor(ch);

        callback(c,type, actor);
    }
}

void yield_camp_cells(const Common::Position& position, std::function<void(Common::Case, Common::GameActor)> callback)
{
    using iter = Common::Position::const_iterator;

    iter camp_cells_begin = position.begin() + camp_cells_begin_offset;
    iter camp_cells_end = position.begin() + camp_cells_end_offset;

    int index = 0;
    const Common::Case camp_cells[4] = {-8, -1, 64, 71};

    for (iter it = camp_cells_begin; it != camp_cells_end; ++it)
    {
        if(*it == '\n')
            continue;

        Common::Case c = camp_cells[index++];

        char ch = *it;
        Common::GameActor actor = to_game_actor(ch);

        callback(c, actor);
    }
}

void yield_current_player(const Common::Position& position, std::function<void(Common::GameActor)> callback)
{
    using iter = Common::Position::const_iterator;

    iter current_player_begin = position.begin() + current_player_begin_offset;

    iter it = current_player_begin;
    Common::GameActor actor;

    switch(*it)
    {
    case '+':
        actor = Common::GameActor::White;
        break;
    case '-':
        actor = Common::GameActor::Black;
        break;
    default:
        actor = Common::GameActor::None;
    };

    callback(actor);
}

void yield_winning_player(const Common::Position& position, std::function<void(Common::GameActor)> callback)
{
    using iter = Common::Position::const_iterator;

    iter winning_player_begin = position.begin() + winning_player_begin_offset;

    iter it = winning_player_begin;
    Common::GameActor actor;

    switch(*it)
    {
    case '+':
        actor = Common::GameActor::White;
        break;
    case '-':
        actor = Common::GameActor::Black;
        break;
    default:
        actor = Common::GameActor::None;
    };

    callback(actor);
}

void yield_pawns(const Common::Position& position, std::function<void(Common::Case, Common::GameActor)> callback)
{
    using iter = Common::Position::const_iterator;

    iter plateau_begin = position.begin();
    iter plateau_end = plateau_begin + 136;

    int index = 0;

    for (iter it = plateau_begin; it != plateau_end; ++it)
    {
        if(*it == '\n')
            continue;

        char c = *(++it);

        switch(c)
        {
        case '+':
            callback(index, Common::GameActor::White);
            break;
        case '-':
            callback(index, Common::GameActor::Black);
            break;
        }

        index++;
    }
}

char from_game_actor(Common::GameActor actor)
{
    switch(actor)
    {
    case Common::GameActor::White:
        return '+';
    case Common::GameActor::Black:
        return '-';
    default:
        return ' ';
    }
}

Common::GameActor to_game_actor(char c)
{
    switch(c)
    {
    case '+':
        return Common::GameActor::White;
    case '-':
        return Common::GameActor::Black;
    default:
        return Common::GameActor::None;
    }
}

char from_cell_type(Common::CellType type)
{
    switch(type)
    {
    case Common::CellType::King:
        return 'K';
    case Common::CellType::Bishop:
        return 'B';
    case Common::CellType::Knight:
        return 'N';
    case Common::CellType::Rook:
        return 'R';
    default:
        return char();
    }
}

Common::CellType to_cell_type(char c)
{
    switch(c)
    {
    case 'K':
        return Common::CellType::King;
    case 'B':
        return Common::CellType::Bishop;
    case 'N':
        return Common::CellType::Knight;
    case 'R':
        return Common::CellType::Rook;
    }

    return  Common::CellType::None;
}

Common::Case move_from(const Common::Move& move)
{
    return std::get<0>(move);
}

Common::Case move_to(const Common::Move& move)
{
    return std::get<1>(move);
}

int case_row(Common::Case c)
{
    return c / 8;
}

int case_col(Common::Case c)
{
    return c % 8;
}

int case_index(Common::Case c)
{
    return c;
}

Common::Case new_case(int row, int column)
{
    return column + (8 * row);
}
