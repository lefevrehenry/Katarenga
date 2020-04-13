#ifndef MESSAGE_UTILS_HPP
#define MESSAGE_UTILS_HPP

#include "MessageWrapper.hpp"
#include "MessageReactor.hpp"

#include "BoardConfigMessage.hpp"
//#include "CheckConnectivity.hpp"
#include "MoveMessage.hpp"
#include "QuitMessage.hpp"
#include "OtherMessage.hpp"
#include "CaseClicked.hpp"

#include <zmqpp/message.hpp>


template <typename T, class ... Args>
zmqpp::message ConstructMessage(Args&&... args)
{
    T object(std::forward<Args>(args)...);

    zmqpp::message message;

    MessageWrapper::MessageType type = T::MessageType();
    message.add(&type, sizeof(MessageWrapper::MessageType));

    object.toMessage(message);

    return message;
}

template <typename T>
T ConstructObject(zmqpp::message& message)
{
    T object;

    const MessageWrapper::MessageType* type;
    message >> type;

    object.fromMessage(message);

    return object;
}

#endif // MESSAGE_UTILS_HPP
