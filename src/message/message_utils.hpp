#ifndef MESSAGE_UTILS_HPP
#define MESSAGE_UTILS_HPP

#include "MessageWrapper.hpp"
#include "MessageReactor.hpp"

#include "BoardConfigMessage.hpp"
#include "CheckConnectivity.hpp"
#include "ReplyConnectivity.hpp"
#include "MoveMessage.hpp"
#include "QuitMessage.hpp"
#include "OtherMessage.hpp"
#include "CaseClicked.hpp"

#include <zmqpp/message.hpp>


template <typename T>
zmqpp::message ConstructMessage(T& object)
{
    zmqpp::message message;

    MessageWrapper::MessageType type = T::MessageType();
    message.add_raw<MessageWrapper::MessageType>(&type, sizeof(MessageWrapper::MessageType));

    object.toMessage(message);

    return message;
}

template <typename T, class ... Args>
zmqpp::message ConstructMessage(Args&&... args)
{
    T object(std::forward<Args>(args)...);

    return ConstructMessage<T>(object);
}

template <typename T>
T ConstructObject(zmqpp::message& message)
{
    T object;

    //message >> type;
    message.next(); // Just skip the first part which is the MessageType

    object.fromMessage(message);

    return object;
}

#endif // MESSAGE_UTILS_HPP
