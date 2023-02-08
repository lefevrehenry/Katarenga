#ifndef KATARENGA_COMMON_SOCKETS_ABSTRACT_SOCKET_HPP
#define KATARENGA_COMMON_SOCKETS_ABSTRACT_SOCKET_HPP

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <memory>

/**
 * @brief The AbstractSocket class defines the interface of a socket
 * This class heavily relies on ZeroMQ library. It was designed to be inherited
 *
 * Classes that derive from AbstractSocket have the tak of defining the behavior
 * of the socket when a message is received
 * The virtual method 'process_input_message' is called when the socket has waiting input
 * It means you should first fetch the message and then do your business on your side. Sending
 * another message is your responsability
 *
 * void process_input_message() override {
 *     zmqpp::message input_message;
 *
 *     receive(input_message);
 *
 *     // specific task you want to do with 'input_message'
 * }
 *
 * In a nutshell you get notify only when a message can be received. The rest is up to you
 *
 * In addition through this class you can retreive:
 *   1. the network endpoint the socket is bind to
 *   2. a pointer to the zmqpp::context in which the socket lives
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
