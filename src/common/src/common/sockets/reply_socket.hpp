#ifndef KATARENGA_COMMON_SOCKETS_REPLY_SOCKET_HPP
#define KATARENGA_COMMON_SOCKETS_REPLY_SOCKET_HPP

// Katarenga
#include <common/engines/reply_engine.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>


/**
 * @brief The ReplySocket class
 */
template< typename T >
class ReplySocket : public zmqpp::socket, public ReplyEngine<T>
{
public:
    ReplySocket(zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
        zmqpp::socket(*context, zmqpp::socket_type::reply)
    {
        bind(endpoint);
    }

public:
    void process() {
        zmqpp::message input_message;

        // receive the request
        receive(input_message);

        // call the callback corresponding to the message type
        // or does nothing if it has not been registered
        zmqpp::message output_message = ReplyEngine<T>::route(input_message);

        // send the reply
        send(output_message);
    }
};

#endif // KATARENGA_COMMON_SOCKETS_REPLY_SOCKET_HPP
