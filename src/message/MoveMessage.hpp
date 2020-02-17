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

static std::string moveTypeToString(const MoveType& type);
static MoveType stringToMoveType(const std::string& type_str);

class MoveMessage: MessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::MoveMessage;
    }

    MoveMessage(zmqpp::message& message);
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getMove() const;
    void setMove(const std::string& move);

    MoveType getType() const;
    void setType(const MoveType& type);

private:
    std::string m_move; // A move string is of the form "current_player:src_index:dest_index" and should be of size 7
    MoveType m_type;
};

#endif
