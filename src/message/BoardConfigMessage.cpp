#include "BoardConfigMessage.hpp"

AnswerBoardConfiguration::AnswerBoardConfiguration(std::string& configuration) : MessageWrapper(), m_configuration(configuration) {}

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
