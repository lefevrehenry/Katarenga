﻿#ifndef KATARENGA_SERVER_SERVER_UTILS_HPP
#define KATARENGA_SERVER_SERVER_UTILS_HPP

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

void msg_server(const std::string& msg);

#endif // KATARENGA_SERVER_UTILS_HPP
