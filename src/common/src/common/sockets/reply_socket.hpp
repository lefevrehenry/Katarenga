#ifndef KATARENGA_COMMON_SOCKETS_REPLY_SOCKET_HPP
#define KATARENGA_COMMON_SOCKETS_REPLY_SOCKET_HPP

// Katarenga
#include <common/sockets/abstract_socket.hpp>
#include <common/engines/reply_engine.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

/**
 * @brief The ReplySocket class
 */
template< typename T >
class ReplySocket : public AbstractSocket, public ReplyEngine<T>
{
public:
    ReplySocket(zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
        AbstractSocket(context, zmqpp::socket_type::reply, endpoint),
        ReplyEngine<T>()
    {
    }

public:
    void process_input_message() {
        zmqpp::message input_message;

        // receive the request
        receive(input_message);

        zmqpp::message output_message = ReplyEngine<T>::process(input_message);

        // send the reply
        send(output_message);
    }

};

#endif // KATARENGA_COMMON_SOCKETS_REPLY_SOCKET_HPP
