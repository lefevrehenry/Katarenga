#ifndef MOVEMESSAGE_HPP
#define MOVEMESSAGE_HPP

#include "MessageWrapper.hpp"
//#include "../server/utils.hpp"

// Standard Library
#include <string>


enum class MoveType {
    PlayThisMove,
    MovePlayed,
    InvalidMove
};

class MoveMessage: MessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::MoveMessage;
    }

public:
    MoveMessage();
    MoveMessage(const MoveType& type, int src, int dest, int player);

public:
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    MoveType getType() const;
    void setType(const MoveType& type);

    int getSource() const;
    void setSource(int index);

    int getDestination() const;
    void setDestination(int index);

    int getPlayer() const;
    void setPlayer(int player);

private:
    MoveType m_type;    // The type of the move
    int m_src;          // The index of the source cell
    int m_dest;         // The index of the destination cell
    int m_player;       // The player who performed the move
};

#endif
