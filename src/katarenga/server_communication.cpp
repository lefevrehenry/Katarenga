#include "server_communication.hpp"
#include <message/message_utils.hpp>

RemoteServer::RemoteServer(const std::string& server_ip, const std::string& server_port) : AbstractServer(),
    m_context(),
    m_socket(m_context, zmqpp::socket_type::request),
    m_poller()
{
    m_socket.connect("tcp://" + server_ip + ":" + server_port);
    m_poller.add(m_socket, zmqpp::poller::poll_in);
}

RemoteServer::~RemoteServer()
{
    m_socket.close();
}

bool RemoteServer::checkServerConnectivity()
{
    CheckConnectivity::Reply reply = communicate<CheckConnectivity>();

    return reply.getConnectivity();
}

std::string RemoteServer::getBoardConfiguration()
{
    BoardConfiguration::Reply reply = communicate<BoardConfiguration>();

    return reply.getConfiguration();
}

template< typename T >
void RemoteServer::send(typename T::Request* request)
{
    MessageType messageType = T::MessageType();

    zmqpp::message message;
    message.add(&messageType, sizeof(MessageType));

    if(request == nullptr)
        return;

    request->toMessage(message);

    m_socket.send(message);
}

template< typename T >
void RemoteServer::receive(typename T::Reply* reply)
{
    // return if nothing is received within the next 5 seconds
    if(!m_poller.poll(RemoteServer::TimeOut))
        return;

    if(m_poller.has_input(m_socket)) {
        zmqpp::message message;
        m_socket.receive(message);

        if(reply == nullptr)
            return;

        reply->fromMessage(message);
    }
}

template< typename T, typename ... Args >
typename T::Reply RemoteServer::communicate(Args&& ... args)
{
    typename T::Request request(std::forward<Args>(args)...);
    send<T>(&request);

    typename T::Reply reply;
    receive<T>(&reply);

    return reply;
}

LocalServer::LocalServer() : AbstractServer()
//    m_context
{
}

bool LocalServer::checkServerConnectivity()
{
    return true;
}

std::string LocalServer::getBoardConfiguration()
{
    return  "";
}
