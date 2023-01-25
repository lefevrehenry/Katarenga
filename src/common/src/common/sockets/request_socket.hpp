#ifndef KATARENGA_COMMON_SOCKETS_REQUEST_SOCKET_HPP
#define KATARENGA_COMMON_SOCKETS_REQUEST_SOCKET_HPP

// Katarenga
#include <common/sockets/abstract_socket.hpp>
#include <common/engines/request_engine.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

/**
 * @brief The RequestSocket class
 */
template< typename T >
class RequestSocket : public AbstractSocket, public RequestEngine<T>
{
public:
    RequestSocket(zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
        AbstractSocket(context, zmqpp::socket_type::request, endpoint),
        RequestEngine<T>()
    {
    }

public:
    void process_input_message() {
        zmqpp::message reply_message;

        // receive the reply
        receive(reply_message);

        RequestEngine<T>::route_reply(reply_message);
    }

    template< typename M >
    void request()
    {
        zmqpp::message request_message = RequestEngine<T>::template route_request<M>();

        // send the request
        send(request_message);

//        process_input_message();  (for synchronous reply)
    }

};

#endif // KATARENGA_COMMON_SOCKETS_REQUEST_SOCKET_HPP
