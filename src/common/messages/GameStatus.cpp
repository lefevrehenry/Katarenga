#include "GameStatus.hpp"


AnswerGameStatus::AnswerGameStatus() : MessageWrapper()
{

}

AnswerGameStatus::AnswerGameStatus(const std::string& board_config, BoardPlayer current_player) : MessageWrapper(),
    m_configuration(board_config),
    m_current_player(current_player)
{

}

void AnswerGameStatus::toMessage(zmqpp::message& message)
{
    message << m_configuration;
    message.add_raw<BoardPlayer>(&m_current_player, sizeof(m_current_player));
}

void AnswerGameStatus::fromMessage(zmqpp::message& message)
{
    message >> m_configuration;
    m_current_player = *message.get<const BoardPlayer*>(2);
}

std::string AnswerGameStatus::getConfiguration() const
{
    return m_configuration;
}

void AnswerGameStatus::setConfiguration(const std::string& configuration)
{
    m_configuration = configuration;
}

BoardPlayer AnswerGameStatus::getCurrentPlayer() const
{
    return m_current_player;
}

void AnswerGameStatus::setCurrentPlayer(BoardPlayer player)
{
    m_current_player = player;
}

