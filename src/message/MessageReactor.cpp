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

template< typename T >
void construct_reply(zmqpp::message& request_message, zmqpp::message& reply_message, MessageReactor::Callback function)
{
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
}

zmqpp::message MessageReactor::process_request(zmqpp::message& request_message) const
{
    // the message returned
    zmqpp::message reply_message;

    // read the header (correspond to the type of the request sent)
    MessageType type = *request_message.get<const MessageType*>(0);

    // try to find the corresponding callback which is tied to the type
    RegistryMap::const_iterator it = m_registry.find(type);

    // if an entry is found in the registry
    if (it != m_registry.end()) {

        // get the corresponding callback
        Callback function = it->second;

        // according to the type of the request we construct the reply message
        switch (type) {
            case MessageType::AskBoardConfiguration: {
                construct_reply<BoardConfiguration>(request_message, reply_message, function);
                break;
            }
            case MessageType::CheckConnectivity: {
                construct_reply<CheckConnectivity>(request_message, reply_message, function);
                break;
            }
            case MessageType::IsThisMoveValid: {
                break;
            }
            case MessageType::PlayThisMove: {
                break;
            }
            case MessageType::ForgetItRageQuit: {
                break;
            }
        }
    }

    return reply_message;
}
