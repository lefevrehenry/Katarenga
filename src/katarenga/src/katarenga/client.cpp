#include "client.hpp"

// Katarenga
#include <common/messages/message.hpp>
#include <common/messages/messages.hpp>
#include <katarenga/client_utils.hpp>

// Standard Library
#include <string>
#include <unistd.h>

Client::Client(const ServerInfo& server_info) :
    m_server_info(server_info),
    m_zmq_context(),
    m_poller(),
    m_connection_socket(this, &m_zmq_context, server_info.processus_endpoint)
{
    m_poller.add(m_connection_socket, zmqpp::poller::poll_in);
    m_poller.add(STDIN_FILENO, zmqpp::poller::poll_in);
}

ConnectionSocket::SPtr Client::connection_socket() const
{
    return nullptr;
}

ServerSocket::SPtr Client::server_socket() const
{
    if(!m_server_socket)
        return nullptr;

    return m_server_socket;
}

void Client::open_server_socket(const zmqpp::endpoint_t& endpoint)
{
    if(m_server_socket)
        return;

    m_server_socket = std::make_shared<ServerSocket>(this, &m_zmq_context, endpoint);

    m_poller.add(*m_server_socket.get(), zmqpp::poller::poll_in);
}

void Client::close_server_socket()
{
    if(!m_server_socket)
        return;

    m_poller.remove(*m_server_socket.get());

    m_server_socket.reset();
}

int Client::exec()
{
    msg_client("Starting main loop of the client");

    msg_client("sending connection request ...");

    {
        m_connection_socket.request<NewConnection>();
    }

    while(true)
    {
        msg_client("poll");
        if(m_poller.poll(5000))//zmqpp::poller::wait_forever))
        {
            if(m_poller.has_input(m_connection_socket))
            {
                m_connection_socket.process_input_message();
            }

            if(m_server_socket && m_poller.has_input(*m_server_socket.get()))
            {
                m_server_socket->process_input_message();
            }

            if(m_poller.has_input(STDIN_FILENO))
            {
                std::string command;
                std::getline(std::cin, command);

                msg_client("command '" + command + "'");

                // process the command read from std::cin
                // process_command_line(command);
            }
        }
        else
        {
            msg_client("server not responding");
            break;
        }
    }

    msg_client("client ending");

    return 0;
}
