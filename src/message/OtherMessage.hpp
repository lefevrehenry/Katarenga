#ifndef OTHERMESSAGE_HPP
#define OTHERMESSAGE_HPP

#include "MessageWrapper.hpp"
//#include "../server/utils.hpp"

// Standard Library
#include <string>


class PlayerWon : MessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::PlayerWon;
    }

    PlayerWon(const int player) : m_player(player) {}
    PlayerWon(zmqpp::message& message);
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    int getPlayer() const;
    void setPlayer(const int player);

private:
    int m_player;

};

/*class PrintBoard : DefaultMessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::PrintBoard;
    }

};*/

#endif
