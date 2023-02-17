#ifndef KATARENGA_COMMON_ENGINES_REQUEST_ENGINE_HPP
#define KATARENGA_COMMON_ENGINES_REQUEST_ENGINE_HPP

// Katarenga
#include <common/messages/message.hpp>

// ZMQPPP
#include <zmqpp/zmqpp.hpp>

/**
 * @brief The RequestEngine class has two main responsabilites
 *
 *   1. make a request
 *   2. react to the reply
 *
 * On the one hand making a request is performed by calling 'process_request<M>'
 * A speciazation of 'execute_request_message' on M is called to get the
 * corresponding payload. It is derived class's responsability to provide this
 * specialization and it must follow the form:
 *
 *    M::Request::Parameters execute_request_message<M>()
 *
 * Afterwards the request stands in a zmqpp::message and is returned to the caller
 *
 * On the other hand a reaction is performed by calling the 'process_reply'
 * A specialization of 'execute_reply_message' on M is called. Once again
 * it is derived class's responsability to provide this specialization
 * and it must follow the form:
 *
 *    void execute_reply_message<M>(M::Reply::Parameters)
 */
template< typename T >
class RequestEngine
{
    using RequestExecutor = zmqpp::message(RequestEngine::*)();
    using RequestMap = std::map<int, RequestExecutor>;

    using ReplyExecutor = void(RequestEngine::*)(const zmqpp::message&);
    using ReplyMap = std::map<int, ReplyExecutor>;

public:
    /**
     * Register the messages you want the engine react to
     * As you register a message 'M' you must provide 2 specializations which are:
     *
     *   1. execute_request_message<M>
     *   2. execute_reply_message<M>
     *
     * See above for the complete signatures of these functions
     */
    template< typename M >
    void registerMessage() {
        request_map[Message::Id<typename M::Request>()] = &RequestEngine<T>::execute_request<M>;
        reply_map[Message::Id<typename M::Reply>()] = &RequestEngine<T>::execute_reply<M>;
    }

protected:
    /**
     * triggers a reaction of the engine
     *
     * here reaction means call 'execute_request_message<M>' specialization
     *
     * then a Kateranga message in the form of a zmqpp::message is returned
     * based on the payload of type M::Request::Parameters
     */
    template< typename M >
    zmqpp::message process_request()
    {
        int id = Message::Id<M>();

        if(request_map.find(id) == request_map.end())
            throw std::runtime_error("Cannot route an unknown message");

        const RequestExecutor& executor = request_map[id];

        if(!executor)
            throw std::runtime_error("Cannot call an unknown executor");

        return (this->*executor)();
    }
    /**
     * a Kateranga message in the form of zmqpp::message that will trigger a reaction
     * of the engine if known or a throw if not
     *
     * here reaction means call 'execute_reply_message<M>' specialization
     */
    void process_reply(const zmqpp::message& input_message)
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
