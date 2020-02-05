#include "OtherMessage.hpp"


PlayerWon::PlayerWon(int player) : MessageWrapper(), m_player(player) {}

void PlayerWon::toMessage(zmqpp::message& message)
{
    message << m_player;
}

void PlayerWon::fromMessage(zmqpp::message& message)
{
    message >> m_player;
}

int PlayerWon::getPlayer() const
{
    return m_player;
}

void PlayerWon::setPlayer(const int player)
{
    m_player = player;
}
