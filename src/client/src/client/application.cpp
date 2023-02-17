#include "application.hpp"

// Katarenga
#include <client/client.hpp>
#include <client/messages/client_messages.hpp>

// Standard Library
#include <unistd.h>
#include <thread>

static int foo(AppInfo app_info)
{
//    Initialize();

    ServerInfo server_info = ReadConfigFile();

    Client client(server_info, app_info);

    return client.exec();
}

Application::Application(const AppInfo& app_info) :
    m_app_info(app_info),
    m_should_quit(false),
    m_poller(),
    m_worker_socket(this, context(), endpoint())
{
    m_poller.add(m_worker_socket, zmqpp::poller::poll_in);
    m_poller.add(STDIN_FILENO, zmqpp::poller::poll_in);
}

zmqpp::context* Application::context() const
{
    return m_app_info.context;
}

zmqpp::endpoint_t Application::endpoint() const
{
    return m_app_info.thread_endpoint;
}

int Application::exec()
{
    std::thread worker(foo, m_app_info);

    while(!m_should_quit)
    {
        std::cout << ">>> " << std::flush;

        if(m_poller.poll(zmqpp::poller::wait_forever))
        {
            if(m_poller.has_input(m_worker_socket))
            {
                m_worker_socket.process_input_message();
            }

            if(m_poller.has_input(STDIN_FILENO))
            {
                std::string command;
                std::getline(std::cin, command);

                // process the command read from std::cin
                process_command_line(command);
            }
        }
    }

    worker.join();

    return 0;
}

void Application::process_command_line(const std::string& command)
{
}
