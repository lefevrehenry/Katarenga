#ifndef MESSAGE_UTILS_HPP
#define MESSAGE_UTILS_HPP

#include "MessageWrapper.hpp"
#include "MessageReactor.hpp"

#include "GameStatus.hpp"
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

    // the first part of the message is the MessageType
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

    // Just skip the first part which is the MessageType
    message.next();

    object.fromMessage(message);

    return object;
}

#endif // MESSAGE_UTILS_HPP
