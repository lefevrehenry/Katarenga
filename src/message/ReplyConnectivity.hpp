#ifndef REPLYCONNECTIVITY_HPP
#define REPLYCONNECTIVITY_HPP

#include "MessageWrapper.hpp"


/**
 * @brief The CheckConnectivity class
 */
class ReplyConnectivity : public MessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::ReplyConnectivity;
    }

public:
    ReplyConnectivity();
    ReplyConnectivity(bool accepted);

public:
    virtual void toMessage(zmqpp::message& message);
    virtual void fromMessage(zmqpp::message& message);

public:
    bool getAccepted() const;
    void setAccepted(bool accepted);

private:
    bool m_accepted;

};

#endif
