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
    using ReplyExecutor = void(RequestEngine::*)(const zmqpp::message&);
    using DoubleExecutor = std::pair<RequestExecutor, ReplyExecutor>;
//    using DoubleExecutor = std::pair<int,int>;

    using RegistryMap = std::map<int, DoubleExecutor>;

public:
    template<typename M>
    void registerMessage() {
        map[Message::Id<M>()] = std::make_pair(&RequestEngine<T>::execute_request<M>, &RequestEngine<T>::execute_reply<M>);
    }

protected:
    template< typename M >
    zmqpp::message route_request() {
        int id = Message::Id<M>();

        if(map.find(id) == map.end())
            throw std::runtime_error("Cannot route an unknown message");

        const RequestExecutor& executor = map[id].first;

        if(!executor)
            throw std::runtime_error("Cannot call an unknown executor");

        return (this->*executor)();
    }

    void route_reply(const zmqpp::message& input_message) {
        int id;
        input_message.get(id, 0);

        if(map.find(id) == map.end())
            throw std::runtime_error("Cannot route an unknown message");

        const ReplyExecutor& executor = map[id].second;

        if(!executor)
            throw std::runtime_error("Cannot call an unknown executor");

        (this->*executor)(input_message);
    }

private:
    template< typename M >
    zmqpp::message execute_request() {
        using Request = typename M::Request;

        using P = typename Request::Parameters;

        T* self = static_cast<T*>(this);
        P parameters = self->template execute_request_message<M>();

        return Message::Create<Request>(parameters);
    }

    template< typename M >
    void execute_reply(const zmqpp::message& input_message) {
        using Reply = typename M::Reply;

        using P = typename Reply::Parameters;
        P parameters = Message::Payload<Reply>(input_message);

        T* self = static_cast<T*>(this);
        self->template execute_reply_message<M>(parameters);
    }

private:
    RegistryMap map;

};

#endif // KATARENGA_COMMON_ENGINES_REQUEST_ENGINE_HPP
