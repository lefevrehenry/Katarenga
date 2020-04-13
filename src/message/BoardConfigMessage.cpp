#include "BoardConfigMessage.hpp"


GameInit::GameInit(zmqpp::message& message) : MessageWrapper() {fromMessage(message);}

void GameInit::toMessage(zmqpp::message& message)
{
    message << m_configuration;
    message << m_current_player;
}

void GameInit::fromMessage(zmqpp::message& message)
{
    message >> m_configuration;
    message >> m_current_player;
}

std::string GameInit::getConfiguration() const
{
    return m_configuration;
}

void GameInit::setConfiguration(const std::string& configuration)
{
    m_configuration = configuration;
}

int GameInit::getCurrentPlayer() const
{
    return m_current_player;
}

void GameInit::setCurrentPlayer(const int player)
{
    m_current_player = player;
}

/*int GameInit::getSelfPlayer() const
{
    return m_self_player;
}

void GameInit::setSelfPlayer(const int player)
{
    m_self_player = player;
}*/





AskBoardConfiguration::AskBoardConfiguration(zmqpp::message &message) : MessageWrapper() {fromMessage(message);}

void AskBoardConfiguration::toMessage(zmqpp::message &message)
{
    message << m_player;
}

void AskBoardConfiguration::fromMessage(zmqpp::message &message)
{
    message >> m_player;
}

int AskBoardConfiguration::getPlayer() const
{
    return m_player;
}

void AskBoardConfiguration::setPlayer(const int player)
{
    m_player = player;
}




AnswerBoardConfiguration::AnswerBoardConfiguration(zmqpp::message& message) : MessageWrapper() {fromMessage(message);}

void AnswerBoardConfiguration::toMessage(zmqpp::message& message)
{
    message << m_configuration;
}

void AnswerBoardConfiguration::fromMessage(zmqpp::message& message)
{
    message >> m_configuration;
}

std::string AnswerBoardConfiguration::getConfiguration() const
{
    return m_configuration;
}

void AnswerBoardConfiguration::setConfiguration(const std::string& configuration)
{
    m_configuration = configuration;
}
