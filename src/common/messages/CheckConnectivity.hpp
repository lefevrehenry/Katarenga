#ifndef CHECKCONNECTIVITY_HPP
#define CHECKCONNECTIVITY_HPP

#include "MessageWrapper.hpp"


/**
 * @brief The CheckConnectivity class
 */
class CheckConnectivity : public DefaultMessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::CheckConnectivity;
    }
/*    CheckConnectivity();

public:
    virtual void toMessage(zmqpp::message& message);
    virtual void fromMessage(zmqpp::message& message);
*/
};

#endif
