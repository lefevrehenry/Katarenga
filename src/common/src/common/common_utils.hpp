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
    using Move = std::tuple<int,int>;

    enum class GameActor {
        None,
        White,
        Black
    };
};

Common::Move convert_to_move(const std::string& str_move);
bool is_valid_index(const Common::Move& move);

#endif // KATARENGA_COMMON_COMMON_UTILS_HPP
