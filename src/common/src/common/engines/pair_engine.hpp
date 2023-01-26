#ifndef KATARENGA_COMMON_ENGINES_PAIR_ENGINE_HPP
#define KATARENGA_COMMON_ENGINES_PAIR_ENGINE_HPP

// Katarenga
#include <common/messages/message.hpp>

// ZMQPPP
#include <zmqpp/zmqpp.hpp>

/**
 * @brief The PairEngine class
 */
template< typename T >
class PairEngine
{
    using ReceiveExecutor = void(PairEngine::*)(const zmqpp::message&);
    using ReceiveMap = std::map<int, ReceiveExecutor>;

    using SendExecutor = zmqpp::message(PairEngine::*)(void);
    using SendMap = std::map<int, SendExecutor>;

public:
    template< typename M >
    void registerReceiveMessage() {
        receive_map[Message::Id<M>()] = &PairEngine<T>::execute_receive<M>;
    }

    template< typename M >
    void registerSendMessage() {
        send_map[Message::Id<M>()] = &PairEngine<T>::execute_send<M>;
    }

protected:
    void route_receive(const zmqpp::message& input_message)
    {
        int id;
        input_message.get(id, 0);

        if(receive_map.find(id) == receive_map.end())
            throw std::runtime_error("Cannot route an unknown message");

        const ReceiveExecutor& executor = receive_map[id];

        if(!executor)
            throw std::runtime_error("Cannot call an unknown executor");

        (this->*executor)(input_message);
    }

    template< typename M >
    zmqpp::message route_send()
    {
        int id = Message::Id<M>();

        if(send_map.find(id) == send_map.end())
            throw std::runtime_error("Cannot route an unknown message");

        const SendExecutor& executor = send_map[id];

        if(!executor)
            throw std::runtime_error("Cannot call an unknown executor");

        return (this->*executor)();
    }

private:
    template<typename M>
    void execute_receive(const zmqpp::message& input_message)
    {
        using P = typename M::Parameters;
        P payload = Message::Payload<M>(input_message);

        T* self = static_cast<T*>(this);
        self->template execute_receive_message<M>(payload);
    }

    template<typename M>
    zmqpp::message execute_send()
    {
        using P = typename M::Parameters;

        T* self = static_cast<T*>(this);
        P payload = self->template execute_send_message<M>();

        return Message::Create<M>(payload);
    }

private:
    ReceiveMap receive_map;
    SendMap send_map;

};

#endif // KATARENGA_COMMON_ENGINES_PAIR_ENGINE_HPP
