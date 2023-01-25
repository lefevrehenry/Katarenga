#ifndef KATARENGA_COMMON_ENGINES_REPLY_ENGINE_HPP
#define KATARENGA_COMMON_ENGINES_REPLY_ENGINE_HPP

// Katarenga
#include <common/messages/message.hpp>

// ZMQPPP
#include <zmqpp/zmqpp.hpp>

/**
 * @brief The ReplyEngine class
 */
template< typename T >
class ReplyEngine
{
    using Executor = zmqpp::message(ReplyEngine::*)(const zmqpp::message&);
    using RegistryMap = std::map<int, Executor>;

public:
    template< typename M >
    void registerMessage() {
        map[Message::Id<typename M::Request>()] = &ReplyEngine<T>::execute<M>;
    }

protected:
    zmqpp::message route(const zmqpp::message& input_message) {
        int id;
        input_message.get(id, 0);

        if(map.find(id) == map.end())
            throw std::runtime_error("Cannot route an unknown message");

        const Executor& executor = map[id];
        if(!executor)
            throw std::runtime_error("Cannot call an unknown executor");

        return (this->*executor)(input_message);
    }

private:
    template< typename M >
    zmqpp::message execute(const zmqpp::message& input_message) {
        using Request = typename M::Request;
        using Reply = typename M::Reply;

        using P = typename Request::Parameters;
        P request = Message::Payload<Request>(input_message);

        T* self = static_cast<T*>(this);
        typename Reply::Parameters reply = self->template execute_message<M>(request);

        return Message::Create<Reply>(reply);
    }

private:
    RegistryMap map;

};

#endif // KATARENGA_COMMON_ENGINES_REPLY_ENGINE_HPP
