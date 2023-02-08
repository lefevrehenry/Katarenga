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
    enum class Status {
        WaitingForReply,
        WaitingForRequest
    };

public:
    RequestSocket(zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
        AbstractSocket(context, zmqpp::socket_type::request, endpoint),
        RequestEngine<T>(),
        m_status(Status::WaitingForRequest)
    {
    }

public:
    void process_input_message()
    {
        if(m_status != Status::WaitingForReply)
            throw std::runtime_error("the socket can't receive a message. First send a request");

        zmqpp::message reply_message;

        // receive the reply
        receive(reply_message);

        RequestEngine<T>::process_reply(reply_message);

        m_status = Status::WaitingForRequest;
    }

    template< typename M >
    void request()
    {
        if(m_status != Status::WaitingForRequest)
            throw std::runtime_error("the socket can't send a message of type '" + Message::Name<M>() + "'. It is waiting for a reply");

        zmqpp::message request_message = RequestEngine<T>::template process_request<typename M::Request>();

        // send the request
        send(request_message);

        m_status = Status::WaitingForReply;

//        process_input_message();  (for synchronous reply)
    }

private:
    Status m_status;

};

#endif // KATARENGA_COMMON_SOCKETS_REQUEST_SOCKET_HPP
