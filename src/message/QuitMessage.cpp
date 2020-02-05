#include "QuitMessage.hpp"


StopGame::StopGame(std::string& reason) : MessageWrapper(), m_reason(reason) {}

void StopGame::toMessage(zmqpp::message& message)
{
    message << m_reason;
}

void StopGame::fromMessage(zmqpp::message& message)
{
    message >> m_reason;
}

std::string StopGame::getReason() const
{
    return m_reason;
}

void StopGame::setReason(const std::string& move)
{
    m_reason = move;
}




GameStopped::GameStopped(std::string& reason) : MessageWrapper(), m_reason(reason) {}

void GameStopped::toMessage(zmqpp::message& message)
{
    message << m_reason;
}

void GameStopped::fromMessage(zmqpp::message& message)
{
    message >> m_reason;
}

std::string GameStopped::getReason() const
{
    return m_reason;
}

void GameStopped::setReason(const std::string& reason)
{
    m_reason = reason;
}
