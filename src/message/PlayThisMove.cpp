#include "PlayThisMove.hpp"

PlayThisMove::Request::Request(const std::string& move) :
    m_move(move)
{
}

void PlayThisMove::Request::toMessage(zmqpp::message& message)
{
    message << m_move;
}

void PlayThisMove::Request::fromMessage(zmqpp::message& message)
{
    message >> m_move;
}

std::string PlayThisMove::Request::getMove() const
{
    return m_move;
}

void PlayThisMove::Request::setMove(const std::string& move)
{
    m_move = move;
}

PlayThisMove::Reply::Reply() :
    m_isAccepted(false)
{

}

void PlayThisMove::Reply::toMessage(zmqpp::message& message)
{
    message << m_isAccepted;
}

void PlayThisMove::Reply::fromMessage(zmqpp::message& message)
{
    message >> m_isAccepted;
}

bool PlayThisMove::Reply::isAccepted() const
{
    return m_isAccepted;
}

void PlayThisMove::Reply::setAccepted(bool isAccepted)
{
    m_isAccepted = isAccepted;
}

PlayThisMove::Broadcast::Broadcast() :
    m_move()
{

}

void PlayThisMove::Broadcast::toMessage(zmqpp::message& message)
{
    message << m_move;
}

void PlayThisMove::Broadcast::fromMessage(zmqpp::message& message)
{
    message >> m_move;
}

std::string PlayThisMove::Broadcast::getMove() const
{
    return m_move;
}

void PlayThisMove::Broadcast::setMove(const std::string& move)
{
    m_move = move;
}
