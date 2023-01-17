#ifndef KATARENGA_COMMON_SOCKETS_PAIR_SOCKET_HPP
#define KATARENGA_COMMON_SOCKETS_PAIR_SOCKET_HPP

// Katarenga
#include <common/engines/pair_engine.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

/**
 * @brief The PairSocket class
 */
template< typename T >
class PairSocket : public zmqpp::socket, public PairEngine<T>
{
public:
    PairSocket(zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
        zmqpp::socket(*context, zmqpp::socket_type::pair)
    {
        bind(endpoint);
    }

public:
    void receive_message() {
        zmqpp::message message;

        // receive the message
        receive(message);

        // call the callback corresponding to the message type
        // or does nothing if it has not been registered
        PairEngine<T>::route(message);
    }

    void send_message() {
//        zmqpp::message message = Message::Create<M>();

//        send(message);
    }
};

#endif // KATARENGA_COMMON_SOCKETS_PAIR_SOCKET_HPP
