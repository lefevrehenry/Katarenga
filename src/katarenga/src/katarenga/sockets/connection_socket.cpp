#include "connection_socket.hpp"

// Katarenga
#include <katarenga/client.hpp>

ConnectionSocket::ConnectionSocket(Client* client, zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
    RequestSocket(context, endpoint),
    m_client(client)
{
    connect(endpoint);
}
