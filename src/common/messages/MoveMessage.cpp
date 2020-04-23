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

MoveMessage::MoveMessage(const MoveType& type, int src, int dest, BoardPlayer player) : MessageWrapper(),
    m_type(type),
    m_src(src),
    m_dest(dest),
    m_player(player)
{

}

void MoveMessage::toMessage(zmqpp::message& message)
{
    message << moveTypeToString(m_type);
    message << m_src;
    message << m_dest;
    message.add_raw<BoardPlayer>(&m_player, sizeof(m_player));
    //message << m_player;
}

void MoveMessage::fromMessage(zmqpp::message& message)
{
    std::string type_str;
    message >> type_str;
    m_type = stringToMoveType(type_str);
    message >> m_src;
    message >> m_dest;
    m_player = *message.get<const BoardPlayer*>(0);
    //message >> m_player;
}

int MoveMessage::getSource() const
{
    return m_src;
}

void MoveMessage::setSource(int index)
{
    m_src = index;
}

int MoveMessage::getDestination() const
{
    return m_dest;
}

void MoveMessage::setDestination(int index)
{
    m_dest = index;
}

MoveType MoveMessage::getType() const
{
    return m_type;
}

void MoveMessage::setType(const MoveType& type)
{
    m_type = type;
}

BoardPlayer MoveMessage::getPlayer() const
{
    return m_player;
}

void MoveMessage::setPlayer(BoardPlayer player)
{
    m_player = player;
}
