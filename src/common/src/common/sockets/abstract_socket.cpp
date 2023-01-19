#include "abstract_socket.hpp"

AbstractSocket::AbstractSocket(zmqpp::context* context, zmqpp::socket_type type, const zmqpp::endpoint_t& endpoint) :
    zmqpp::socket(*context, type),
    m_zmq_context(context),
    m_endpoint(endpoint)
{
    bind(endpoint);
}
