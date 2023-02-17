#ifndef KATARENGA_COMMON_ENGINES_REPLY_ENGINE_HPP
#define KATARENGA_COMMON_ENGINES_REPLY_ENGINE_HPP

// Katarenga
#include <common/messages/message.hpp>

// ZMQPPP
#include <zmqpp/zmqpp.hpp>

/**
 * @brief The ReplyEngine class aims to reply after a request
 *
 * Reaction is performed by calling the 'process' function
 * A specialization of 'execute_message' on M is called. It is derived
 * class's responsability to provide this specialization and it must
 * follow the form:
 *
 *    M::Reply::Parameters execute_message<M>(const M::Request::Parameters& parameters)
 *
 * The returned payload will be used to construct the reply of M which will stands in
 * a zmqpp::message that is returned to the caller
 *
 *
 *
 *
 * The engines react to an incoming message by calling the specialization of
 * the 'execute_message' function which is intended to be specified in their template
 *
 * Each engine defines its own expectations of dialogue through the function 'execute_message'
 * Should it return a payload or not is up to them. They do not receive or send a message
 * In other words their roles stops at reacting based on the type of the message we give them
 *
 * Reaction goes through the template function 'execute_message' which should
 * be specialized for each Katarenga message we want to support
 *
 * We speak with the engines with Katarenga message only. They hide for us the complexity
 * of the zmqpp::message and their formatting to let us focused on business we want to do
 * with the Katarenga message
 * It unwraps an incoming message 'M' to present its payload (M::Parameters) through the
 * 'execute_message' function. Eventually the engines expect back a payload in return
 * from 'execute_message' if needed (depend on the logic of each enigne)
 *
 * The engine can only react on a set of message which should be registered first
 * Call registerMessage<M> with 'M' the message you want to registered
 */
template< typename T >
class ReplyEngine
{
    using Executor = zmqpp::message(ReplyEngine::*)(const zmqpp::message&);
    using RegistryMap = std::map<int, Executor>;

public:
    /**
     * Register the messages you want the engine react to
     * As you register a message 'M' you must provide a specialization which is:
     *
     *   1. execute_message<M>
     *
     * See above for the complete signatures of this function
     */
    template< typename M >
    void registerMessage() {
        map[Message::Id<typename M::Request>()] = &ReplyEngine<T>::execute<M>;
    }

protected:
    /**
     * a Kateranga message in the form of zmqpp::message that will trigger a reaction
     * of the engine if known or a throw if not
     *
     * here reaction means call 'execute_message<M>' specialization
     */
    zmqpp::message process(const zmqpp::message& input_message) {
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
