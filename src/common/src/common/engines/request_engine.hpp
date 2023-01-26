#ifndef KATARENGA_COMMON_ENGINES_REQUEST_ENGINE_HPP
#define KATARENGA_COMMON_ENGINES_REQUEST_ENGINE_HPP

// Katarenga
#include <common/messages/message.hpp>

// ZMQPPP
#include <zmqpp/zmqpp.hpp>

/**
 * @brief The RequestEngine class
 */
template< typename T >
class RequestEngine
{
    using RequestExecutor = zmqpp::message(RequestEngine::*)();
    using RequestMap = std::map<int, RequestExecutor>;

    using ReplyExecutor = void(RequestEngine::*)(const zmqpp::message&);
    using ReplyMap = std::map<int, ReplyExecutor>;

public:
    template< typename M >
    void registerMessage() {
        request_map[Message::Id<typename M::Request>()] = &RequestEngine<T>::execute_request<M>;
        reply_map[Message::Id<typename M::Reply>()] = &RequestEngine<T>::execute_reply<M>;
    }

protected:
    template< typename M >
    zmqpp::message route_request()
    {
        int id = Message::Id<M>();

        if(request_map.find(id) == request_map.end())
            throw std::runtime_error("Cannot route an unknown message");

        const RequestExecutor& executor = request_map[id];

        if(!executor)
            throw std::runtime_error("Cannot call an unknown executor");

        return (this->*executor)();
    }

    void route_reply(const zmqpp::message& input_message)
    {
        int id;
        input_message.get(id, 0);

        if(reply_map.find(id) == reply_map.end())
            throw std::runtime_error("Cannot route an unknown message");

        const ReplyExecutor& executor = reply_map[id];

        if(!executor)
            throw std::runtime_error("Cannot call an unknown executor");

        (this->*executor)(input_message);
    }

private:
    template< typename M >
    zmqpp::message execute_request()
    {
        using Request = typename M::Request;

        using P = typename Request::Parameters;

        T* self = static_cast<T*>(this);
        P payload = self->template execute_request_message<M>();

        return Message::Create<Request>(payload);
    }

    template< typename M >
    void execute_reply(const zmqpp::message& input_message)
    {
        using Reply = typename M::Reply;

        using P = typename Reply::Parameters;
        P payload = Message::Payload<Reply>(input_message);

        T* self = static_cast<T*>(this);
        self->template execute_reply_message<M>(payload);
    }

private:
    RequestMap request_map;
    ReplyMap reply_map;

};

#endif // KATARENGA_COMMON_ENGINES_REQUEST_ENGINE_HPP
