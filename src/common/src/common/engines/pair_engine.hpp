#ifndef KATARENGA_COMMON_ENGINES_PAIR_ENGINE_HPP
#define KATARENGA_COMMON_ENGINES_PAIR_ENGINE_HPP

// Katarenga
#include <common/messages/messages.hpp>

// ZMQPPP
#include <zmqpp/zmqpp.hpp>

/**
 * @brief The ReplyEngine class
 */
template< typename T >
class PairEngine
{
    using Executor = void(PairEngine::*)(const zmqpp::message&);
    using RegistryMap = std::map<int, Executor>;

public:
    template<typename M>
    void registerMessage() {
        map[Message::Id<typename M::Request>()] = &PairEngine<T>::execute<M>;
    }

protected:
    void route(const zmqpp::message& input_message) {
        int id;
        input_message.get(id, 0);

        if(map.find(id) == map.end())
            throw std::runtime_error("Cannot route an unknown message");

        const Executor& executor = map[id];
        if(!executor)
            throw std::runtime_error("Cannot call an unknown executor");

        (this->*executor)(input_message);
    }

private:
    template<typename M>
    void execute(const zmqpp::message& input_message) {

        using P = typename M::Parameters;
        P payload = Message::Payload<M>(input_message);

        T* self = static_cast<T*>(this);
        self->template execute_message<M>(payload);
    }

private:
    RegistryMap map;

};

#endif // KATARENGA_COMMON_ENGINES_PAIR_ENGINE_HPP
