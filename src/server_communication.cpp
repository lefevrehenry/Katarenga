#include "server_communication.hpp"

RemoteServer::RemoteServer(const std::string& server_ip, const std::string& server_port) : AbstractServer(),
    m_context(),
    m_socket(m_context, zmqpp::socket_type::pair),
    m_poller()
{
    m_socket.connect("tcp://" + server_ip + ":" + server_port);
    m_poller.add(m_socket, zmqpp::poller::poll_in);
}

RemoteServer::~RemoteServer()
{
    m_socket.close();
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
    // return if nothing is received within the next few seconds
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

bool RemoteServer::checkServerConnectivity()
{
    CheckConnectivity::Reply reply = communicate<CheckConnectivity>();

    return reply.getConnectivity();

//    zmqpp::poller poller;
//    poller.add(m_socket, zmqpp::poller::poll_in);

//    zmqpp::message request;
//    request << "ping";

//    m_socket.send(request, true);

//    zmqpp::message reply;
//    m_socket.receive(reply);

//    std::string string_reply;
//    reply >> string_reply;

//    return string_reply == "pong";
}

std::string RemoteServer::getBoardConfiguration()
{
    BoardConfiguration::Reply reply = communicate<BoardConfiguration>();

    return reply.getConfiguration();
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
