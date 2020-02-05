#include "MoveMessage.hpp"



PlayThisMove::PlayThisMove(std::string& move) : MessageWrapper(), m_move(move) {}

void PlayThisMove::toMessage(zmqpp::message& message)
{
    message << m_move;
}

void PlayThisMove::fromMessage(zmqpp::message& message)
{
    message >> m_move;
}

std::string PlayThisMove::getMove() const
{
    return m_move;
}

void PlayThisMove::setMove(const std::string& move)
{
    m_move = move;
}


MovePlayed::MovePlayed(std::string& move) : MessageWrapper(), m_move(move) {}

void MovePlayed::toMessage(zmqpp::message& message)
{
    message << m_move;
}

void MovePlayed::fromMessage(zmqpp::message& message)
{
    message >> m_move;
}

std::string MovePlayed::getMove() const
{
    return m_move;
}

void MovePlayed::setMove(const std::string& move)
{
    m_move = move;
}
