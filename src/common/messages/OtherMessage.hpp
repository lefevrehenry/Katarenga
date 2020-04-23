#ifndef OTHERMESSAGE_HPP
#define OTHERMESSAGE_HPP

#include "MessageWrapper.hpp"
#include "../board/board_utils.hpp"

// Standard Library
#include <string>


class PlayerWon : MessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::PlayerWon;
    }

public:
    PlayerWon();
    PlayerWon(BoardPlayer player);

public:
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    BoardPlayer getPlayer() const;
    void setPlayer(BoardPlayer player);

private:
    BoardPlayer m_player;

};


#endif
