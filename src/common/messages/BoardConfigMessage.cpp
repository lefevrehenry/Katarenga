#include "BoardConfigMessage.hpp"


GameInit::GameInit() : MessageWrapper()
{

}

GameInit::GameInit(const std::string& board_config, BoardPlayer current_player) : MessageWrapper(),
    m_configuration(board_config),
    m_current_player(current_player)
{

}

void GameInit::toMessage(zmqpp::message& message)
{
    message << m_configuration;
    message.add_raw<BoardPlayer>(&m_current_player, sizeof(m_current_player));
    //message << m_current_player;
}

void GameInit::fromMessage(zmqpp::message& message)
{
    message >> m_configuration;
    m_current_player = *message.get<const BoardPlayer*>(0);
    //message >> m_current_player;
}

std::string GameInit::getConfiguration() const
{
    return m_configuration;
}

void GameInit::setConfiguration(const std::string& configuration)
{
    m_configuration = configuration;
}

BoardPlayer GameInit::getCurrentPlayer() const
{
    return m_current_player;
}

void GameInit::setCurrentPlayer(BoardPlayer player)
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




/*
AskBoardConfiguration::AskBoardConfiguration() : MessageWrapper()
{

}

AskBoardConfiguration::AskBoardConfiguration(int player) : MessageWrapper(),
    m_player(player)
{

}

void AskBoardConfiguration::toMessage(zmqpp::message& message)
{
    message << m_player;
}

void AskBoardConfiguration::fromMessage(zmqpp::message& message)
{
    message >> m_player;
}

int AskBoardConfiguration::getPlayer() const
{
    return m_player;
}

void AskBoardConfiguration::setPlayer(int player)
{
    m_player = player;
}

*/


AnswerBoardConfiguration::AnswerBoardConfiguration() : MessageWrapper()
{

}

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
