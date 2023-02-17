#ifndef KATARENGA_COMMON_ENGINES_PAIR_ENGINE_HPP
#define KATARENGA_COMMON_ENGINES_PAIR_ENGINE_HPP

// Katarenga
#include <common/messages/message.hpp>

// ZMQPPP
#include <zmqpp/zmqpp.hpp>

/**
 * @brief The PairEngine class aims to freely received or sent message without
 * any particular assumptions
 *
 * A reaction on a received message is performed by calling 'process_receive'
 * A speciazation of 'execute_receive_message' on M is called. It is derived
 * class's responsability to provide this specialization and it must follow
 * the form:
 *
 *    void execute_receive_message<M>(M::Parameters)
 *
 * And making a message is performed by calling 'process_send<M>'
 * A speciazation of 'execute_send_message' on M is called to get the
 * corresponding payload. It is derived class's responsability to provide this
 * specialization and it must follow the form:
 *
 *    M::Parameters execute_send_message<M>()
 *
 * Afterwards the message stands in a zmqpp::message and is returned to the caller
 */
template< typename T >
class PairEngine
{
    using ReceiveExecutor = void(PairEngine::*)(const zmqpp::message&);
    using ReceiveMap = std::map<int, ReceiveExecutor>;

    using SendExecutor = zmqpp::message(PairEngine::*)(void);
    using SendMap = std::map<int, SendExecutor>;

public:
    /**
     * Register the messages you want the engine react to
     * As you register a receive message 'M' you must provide a specialization which is:
     *
     *   1. execute_receive_message<M>
     *
     * See above for the complete signatures of these functions
     */
    template< typename M >
    void registerReceiveMessage() {
        receive_map[Message::Id<M>()] = &PairEngine<T>::execute_receive<M>;
    }
    /**
     * Register the messages you want the engine react to
     * As you register a receive message 'M' you must provide a specialization which is:
     *
     *   1. execute_receive_message<M>
     *
     * See above for the complete signatures of these functions
     */
    template< typename M >
    void registerSendMessage() {
        send_map[Message::Id<M>()] = &PairEngine<T>::execute_send<M>;
    }

protected:
    /**
     * a Kateranga message in the form of zmqpp::message that will trigger a reaction
     * of the engine if known or a throw if not
     *
     * here reaction means call 'execute_receive_message<M>' specialization
     */
    void process_receive(const zmqpp::message& input_message)
    {
        int id;
        input_message.get(id, 0);

        if(receive_map.find(id) == receive_map.end())
            throw std::runtime_error("Cannot route_receive an unknown message (" + std::to_string(id) + ")");

        const ReceiveExecutor& executor = receive_map[id];

        if(!executor)
            throw std::runtime_error("Cannot call an unknown executor");

        (this->*executor)(input_message);
    }
    /**
     * triggers a reaction of the engine
     *
     * here reaction means call 'execute_send_message<M>' specialization
     *
     * then a Kateranga message in the form of a zmqpp::message is returned
     * based on the payload of type M::Parameters
     */
    template< typename M >
    zmqpp::message process_send()
    {
        int id = Message::Id<M>();

        if(send_map.find(id) == send_map.end())
            throw std::runtime_error("Cannot route_send an unknown message (" + std::to_string(id) + ")");

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
