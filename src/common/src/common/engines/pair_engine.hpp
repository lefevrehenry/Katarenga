#ifndef KATARENGA_COMMON_ENGINES_PAIR_ENGINE_HPP
#define KATARENGA_COMMON_ENGINES_PAIR_ENGINE_HPP

// Katarenga
#include <common/messages/messages.hpp>

// ZMQPPP
#include <zmqpp/zmqpp.hpp>

/**
 * @brief The PairEngine class
 */
template< typename T >
class PairEngine
{
    using ReceiveExecutor = void(PairEngine::*)(const zmqpp::message&);
    using SendExecutor = zmqpp::message(PairEngine::*)(void);
    using Executors = std::pair<ReceiveExecutor, SendExecutor>;

    using RegistryMap = std::map<int, Executors>;

public:
    template<typename M>
    void registerMessage() {
        map[Message::Id<M>()] = std::make_pair(&PairEngine<T>::execute_receive<M>, &PairEngine<T>::execute_send<M>);
    }

protected:
    void route_receive(const zmqpp::message& input_message) {
        int id;
        input_message.get(id, 0);

        if(map.find(id) == map.end())
            throw std::runtime_error("Cannot route an unknown message");

        const ReceiveExecutor& executor = map[id].first;
        if(!executor)
            throw std::runtime_error("Cannot call an unknown executor");

        (this->*executor)(input_message);
    }

//    zmqpp::message route_send(int id) {
//        if(map.find(id) == map.end())
//            throw std::runtime_error("Cannot route an unknown message");

//        const SendExecutor& executor = map[id].second;
//        if(!executor)
//            throw std::runtime_error("Cannot call an unknown executor");

//        (this->*executor)();
//    }
private:
    template<typename M>
    void execute_receive(const zmqpp::message& input_message) {

        using P = typename M::Parameters;
        P payload = Message::Payload<M>(input_message);

        T* self = static_cast<T*>(this);
        self->template execute_receive_message<M>(payload);
    }

    template<typename M>
    zmqpp::message execute_send() {
        using P = typename M::Parameters;
        P payload;

//        T* self = static_cast<T*>(this);
//        self->template execute_send_message<M>(&payload);

        return Message::Create<M>(payload);
    }

private:
    RegistryMap map;

};

#endif // KATARENGA_COMMON_ENGINES_PAIR_ENGINE_HPP
