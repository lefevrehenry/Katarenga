#include "application_socket.hpp"

// Katarenga
#include <client/client.hpp>
#include <client/messages/client_messages.hpp>

ApplicationSocket::ApplicationSocket(Client* client, zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
    PairSocket(context, endpoint),
    m_client(client)
{
    bind(endpoint);

    registerReceiveMessage<Quit>();
}

template<>
void ApplicationSocket::execute_receive_message<Quit>(const typename Quit::Parameters& parameters)
{
    m_client->quit();
}
