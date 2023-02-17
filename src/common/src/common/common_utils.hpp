#ifndef KATARENGA_COMMON_COMMON_UTILS_HPP
#define KATARENGA_COMMON_COMMON_UTILS_HPP

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <string>
#include <tuple>

class Board;

struct ServerInfo
{
    /**
     * the zmq context used for the server
     */
    zmqpp::context* context;

    /**
     * the socket entry point for inter-threads communication
     */
    zmqpp::endpoint_t thread_endpoint;

    /**
     * the socket entry point for inter-proccess communication
     */
    zmqpp::endpoint_t processus_endpoint;

    /**
     * the socket entry point for communication over the network
     */
    zmqpp::endpoint_t network_endpoint;
};

ServerInfo ReadConfigFile();

void Initialize();

/**
 * @brief The Common struct
 */
struct Common
{
    using GameId = unsigned int;
    using Case = int;
    using Position = std::string;

    using From = Case;
    using To = Case;
    using Move = std::tuple<From,To>;

    enum class GameActor {
        None,
        White,
        Black
    };

    enum CellType {
        None,
        King,
        Rook,
        Bishop,
        Knight,
        CampCell
    };
};

Common::Case new_case(int row, int column);

int case_row(Common::Case c);
int case_col(Common::Case c);
int case_index(Common::Case c);

Common::Case move_from(const Common::Move& move);
Common::Case move_to(const Common::Move& move);

char from_game_actor(Common::GameActor actor);
Common::GameActor to_game_actor(char c);

char from_cell_type(Common::CellType type);
Common::CellType to_cell_type(char c);

bool has_valid_format(const Common::Position& position);

void yield_cases(const Common::Position& position, std::function<void(Common::Case,Common::CellType,Common::GameActor)> callback);
void yield_camp_cells(const Common::Position& position, std::function<void(Common::Case,Common::GameActor)> callback);
void yield_current_player(const Common::Position& position, std::function<void(Common::GameActor)> callback);
void yield_winning_player(const Common::Position& position, std::function<void(Common::GameActor)> callback);
void yield_pawns(const Common::Position& position, std::function<void(Common::Case,Common::GameActor)> callback);

Common::Move convert_to_move(const std::string& str_move);
bool is_valid_index(const Common::Move& move);

#endif // KATARENGA_COMMON_COMMON_UTILS_HPP
