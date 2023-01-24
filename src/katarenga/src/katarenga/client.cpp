#include "client.hpp"

// Katarenga
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

int Client::exec()
{
    msg_client("Starting main loop of the client");

    msg_client("sending connection request ...");

    {
        NewConnection::Request::Parameters R = {"jacky", "0.0.0.1", "28000"};
        zmqpp::message request = Message::Create<NewConnection::Request>(R);

        m_connection_socket.send(request);
    }

    while(true)
    {
        msg_client("poll");
        if(m_poller.poll(5000))//zmqpp::poller::wait_forever))
        {
            if(m_poller.has_input(m_connection_socket))
            {
                zmqpp::message reply;
                m_connection_socket.receive(reply);
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
