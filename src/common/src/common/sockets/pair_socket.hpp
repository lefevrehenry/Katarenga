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
        PairEngine<T>::route_receive(message);
    }

    template< typename M >
    void send_message() {
        int id = Message::Id<M>();

        zmqpp::message message = PairEngine<T>::route_send(id);

        // send the message
        send(message);
    }
};

#endif // KATARENGA_COMMON_SOCKETS_PAIR_SOCKET_HPP
