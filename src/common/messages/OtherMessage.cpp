#include "OtherMessage.hpp"


PlayerWon::PlayerWon() : MessageWrapper()
{

}

PlayerWon::PlayerWon(BoardPlayer player) : MessageWrapper(),
    m_player(player)
{

}

void PlayerWon::toMessage(zmqpp::message& message)
{
    //message << m_player;
    message.add_raw<BoardPlayer>(&m_player, sizeof(m_player));
}

void PlayerWon::fromMessage(zmqpp::message& message)
{
    //message >> m_player;
    m_player = *message.get<const BoardPlayer*>(1);
}

BoardPlayer PlayerWon::getPlayer() const
{
    return m_player;
}

void PlayerWon::setPlayer(BoardPlayer player)
{
    m_player = player;
}
