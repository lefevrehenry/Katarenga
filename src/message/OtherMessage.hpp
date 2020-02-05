#ifndef OTHERMESSAGE_HPP
#define OTHERMESSAGE_HPP

#include "MessageWrapper.hpp"
//#include "../server/utils.hpp"

// Standard Library
#include <string>


class PlayerWon : MessageWrapper
{
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::PlayerWon;
    }

public:
    PlayerWon(int player);
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    int getPlayer() const;
    void setPlayer(const int player);

private:
    int m_player;

};

class PrintBoard : DefaultMessageWrapper
{
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::PrintBoard;
    }

public:
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

};

#endif
