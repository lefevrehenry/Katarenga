#include "MoveMessage.hpp"

#include <stdexcept>

static std::string moveTypeToString(const MoveType& type)
{
    std::string s;
    switch(type)
    {
    case MoveType::PlayThisMove:
        s = "PlayThisMove";
        break;
    case MoveType::MovePlayed:
        s = "MovePlayed";
        break;
    case MoveType::InvalidMove:
        s = "InvalidMove";
        break;
    default:
        s = "UnknownMoveType";
    }
    return s;
}

static MoveType stringToMoveType(const std::string& move_str)
{
    MoveType type;
    if (move_str == "PlayThisMove")
        type = MoveType::PlayThisMove;
    else if (move_str == "MovePlayed")
        type = MoveType::MovePlayed;
    else if (move_str == "InvalidMove")
        type = MoveType::InvalidMove;
    else
        throw std::runtime_error("Unknown move type string received");
    return type;
}

MoveMessage::MoveMessage() : MessageWrapper()
{

}

void MoveMessage::toMessage(zmqpp::message& message)
{
    message << moveTypeToString(m_type);
    message << m_move;
    message << m_player;
}

void MoveMessage::fromMessage(zmqpp::message& message)
{
    std::string type_str;
    message >> type_str;
    m_type = stringToMoveType(type_str);
    message >> m_move;
    message >> m_player;
}

std::string MoveMessage::getMove() const
{
    return m_move;
}

void MoveMessage::setMove(const std::string& move)
{
    m_move = move;
}

MoveType MoveMessage::getType() const
{
    return m_type;
}

void MoveMessage::setType(const MoveType& type)
{
    m_type = type;
}

int MoveMessage::getPlayer() const
{
    return m_player;
}

void MoveMessage::setPlayer(int player)
{
    m_player = player;
}
