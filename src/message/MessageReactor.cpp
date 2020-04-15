#include "MessageReactor.hpp"
#include "message_utils.hpp"

MessageReactor::MessageReactor() :
    m_registry()
{

}

void MessageReactor::add(MessageType type, Callback callback)
{
    m_registry[type] = callback;
}

void MessageReactor::remove(MessageReactor::MessageType type)
{
    RegistryMap::const_iterator it = m_registry.find(type);

    if (it != m_registry.end())
        m_registry.erase(it);
}

bool MessageReactor::process_message(zmqpp::message& message) const
{
    // read the header (correspond to the type of the message sent)
    MessageType type = *message.get<const MessageType*>(0);

    // try to find the corresponding callback which is tied to this type
    RegistryMap::const_iterator it = m_registry.find(type);

    // if an entry is found in the registry
    if (it != m_registry.end())
    {
        // get the corresponding callback
        Callback function = it->second;

        // call it
        function(message);

        return true;
    }

    return false;
}

//zmqpp::message MessageReactor::process_broadcast(zmqpp::message& request_message, zmqpp::message& reply_message) const
//{
//    // the message returned
//    zmqpp::message publish_message;

//    // read the header (correspond to the type of the request sent)
//    MessageType type = *request_message.get<const MessageType*>(0);

//    // according to the type of the request we construct the publish message
//    switch (type) {
//        case MessageType::PlayThisMove: {
//            publish_message = construct_publish<PlayThisMove>(request_message, reply_message);
//            break;
//        }
//        case MessageType::ForgetItRageQuit: {
//            break;
//        }
//        default: {
//            break;
//        }
//    }

//    return publish_message;
//}
