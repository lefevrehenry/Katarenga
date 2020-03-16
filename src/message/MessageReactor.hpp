#ifndef MESSAGEREACTOR_HPP
#define MESSAGEREACTOR_HPP

#include "MessageWrapper.hpp"

// ZMQ
#include <zmqpp/message.hpp>

// Standard Library
#include <map>
#include <functional>


/**
 * @brief The MessageReactor class register callbacks to be called depending on a request message's type
 */
class MessageReactor
{

public:
    using MessageType = MessageWrapper::MessageType;
    using Callback = std::function<void(zmqpp::message& message)>;

private:
    using RegistryMap = std::map<MessageType, Callback>;

public:
    MessageReactor();

public:
    template< typename T >
    static zmqpp::message ConstructReply(zmqpp::message& request_message, std::function<void(typename T::Request*, typename T::Reply*)> function)
    {
        // the message returned
        zmqpp::message reply_message;

        // reconstruct the request object from the (input) request message
        typename T::Request request;
        request.fromMessage(request_message);

        // construct the reply object for the (output) reply message
        typename T::Reply reply;

        // call the callback
        // the callback is responsible to fulfill the reply object
        function(&request, &reply);

        // write the output message from the reply object
        reply.toMessage(reply_message);

        return reply_message;
    }

public:
    void add(MessageType type, Callback callback);
    void remove(MessageType type);

public:
    void process_message(zmqpp::message& message) const;
//    zmqpp::message process_broadcast(zmqpp::message& request_message, zmqpp::message& reply_message) const;

private:
    RegistryMap m_registry;

};

#endif
