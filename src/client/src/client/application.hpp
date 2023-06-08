#ifndef KATARENGA_CLIENT_APPLICATION_HPP
#define KATARENGA_CLIENT_APPLICATION_HPP

// Katarenga
#include <client/client_utils.hpp>
#include <client/sockets/worker_socket.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <string>

/**
 * @brief The Application class
 */
class Application
{

public:
    Application(const AppInfo& app_info);

public:
    zmqpp::context* context() const;
    zmqpp::endpoint_t endpoint() const;

public:
    int exec();

private:
    void process_command_line(const std::string& command);

private:
    // Application-related content
    AppInfo         m_app_info;
    bool            m_should_quit;

    // Socket-related content
    zmqpp::poller   m_poller;

    WorkerSocket    m_worker_socket;

};

#endif // KATARENGA_CLIENT_APPLICATION_HPP
