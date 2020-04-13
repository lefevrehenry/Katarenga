#include "QuitMessage.hpp"


StopGame::StopGame() : MessageWrapper()
{

}

StopGame::StopGame(const std::string& reason, int player) : MessageWrapper(),
    m_reason(reason),
    m_player(player)
{

}

void StopGame::toMessage(zmqpp::message& message)
{
    message << m_reason;
    message << m_player;
}

void StopGame::fromMessage(zmqpp::message& message)
{
    message >> m_reason;
    message >> m_player;
}

std::string StopGame::getReason() const
{
    return m_reason;
}

void StopGame::setReason(const std::string& reason)
{
    m_reason = reason;
}

int StopGame::getPlayer() const
{
    return m_player;
}

void StopGame::setPlayer(int player)
{
    m_player = player;
}


GameStopped::GameStopped() : MessageWrapper()
{

}

GameStopped::GameStopped(const std::string& reason) : MessageWrapper(),
    m_reason(reason)
{

}

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
