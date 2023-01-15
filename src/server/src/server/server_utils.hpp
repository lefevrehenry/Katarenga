#ifndef KATARENGA_SERVER_SERVER_UTILS_HPP
#define KATARENGA_SERVER_SERVER_UTILS_HPP

// ZMQPP
#include <zmqpp/zmqpp.hpp>

struct ServerInfo
{
    zmqpp::endpoint_t tcp_endpoint;           // the tcp entry point for new connections from the network
    zmqpp::endpoint_t localhost_endpoint;     // the local socket entry point for inter-proccess communication
};

void server_msg(const std::string& msg);

#endif // KATARENGA_SERVER_UTILS_HPP
