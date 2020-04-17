#ifndef CHECKCONNECTIVITY_HPP
#define CHECKCONNECTIVITY_HPP

#include "MessageWrapper.hpp"


/**
 * @brief The CheckConnectivity class
 */
class CheckConnectivity : public MessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::CheckConnectivity;
    }

public:
    CheckConnectivity();
    CheckConnectivity(int player);

public:
    virtual void toMessage(zmqpp::message& message);
    virtual void fromMessage(zmqpp::message& message);

public:
    int getPlayer() const;
    void setPlayer(int player);

private:
    int m_player;

};

#endif
