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
    MoveMessage(const MoveType& type, const std::string& move, int player);

public:
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    MoveType getType() const;
    void setType(const MoveType& type);

    std::string getMove() const;
    void setMove(const std::string& move);

    int getPlayer() const;
    void setPlayer(int player);

private:
    MoveType m_type;    // The type of the move
    std::string m_move; // A move string is of the form "src_index:dest_index" and should be of size 5
    int m_player;       // The player who performed the move
};

#endif
