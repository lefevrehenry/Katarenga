#ifndef KATARENGA_CLIENT_CLIENT_UTILS_HPP
#define KATARENGA_CLIENT_CLIENT_UTILS_HPP

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <string>

struct AppInfo
{
    /**
     * the zmq context used for inter-proccess communication
     */
    zmqpp::context* context;

    /**
     * the socket entry point for inter-threads communication
     */
    zmqpp::endpoint_t thread_endpoint;
};

void InitializeClient();

void msg_client(const std::string& msg);
void warning_client(const std::string& msg);

#endif // KATARENGA_CLIENT_CLIENT_UTILS_HPP
