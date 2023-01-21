#ifndef KATARENGA_COMMON_SOCKETS_ABSTRACT_SOCKET_HPP
#define KATARENGA_COMMON_SOCKETS_ABSTRACT_SOCKET_HPP

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <memory>

/**
 * @brief The AbstractSocket class
 */
class AbstractSocket : public zmqpp::socket
{
public:
    using SPtr = std::shared_ptr<AbstractSocket>;

public:
    AbstractSocket(zmqpp::context* context, zmqpp::socket_type type, const zmqpp::endpoint_t& endpoint);

public:
    virtual void process_input_message() = 0;

public:
    zmqpp::context* context() const { return m_zmq_context; }
    zmqpp::endpoint_t endpoint() const { return m_endpoint; }

private:
    zmqpp::context* m_zmq_context;
    zmqpp::endpoint_t m_endpoint;
};

#endif // KATARENGA_COMMON_SOCKETS_ABSTRACT_SOCKET_HPP
