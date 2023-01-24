#ifndef KATARENGA_COMMON_COMMON_UTILS_HPP
#define KATARENGA_COMMON_COMMON_UTILS_HPP

// ZMQPP
#include <zmqpp/zmqpp.hpp>

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

/**
 * @brief The Common struct
 */
struct Common
{
    using GameId = unsigned int;

    enum class GameActor {
        White,
        Black,
        Default
    };
};

#endif // KATARENGA_COMMON_COMMON_UTILS_HPP
