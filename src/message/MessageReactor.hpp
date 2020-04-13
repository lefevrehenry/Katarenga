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
    void add(MessageType type, Callback callback);
    void remove(MessageType type);

public:
    void process_message(zmqpp::message& message) const;

private:
    RegistryMap m_registry;

};

#endif
