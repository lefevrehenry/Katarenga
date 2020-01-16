#include "BoardConfiguration.hpp"

BoardConfiguration::Reply::Reply() : MessageWrapper(),
    m_configuration("")
{
}

void BoardConfiguration::Reply::toMessage(zmqpp::message& message)
{
    message << m_configuration;
}

void BoardConfiguration::Reply::fromMessage(zmqpp::message& message)
{
    message >> m_configuration;
}

std::string BoardConfiguration::Reply::getConfiguration() const
{
    return m_configuration;
}

void BoardConfiguration::Reply::setConfiguration(const std::string& configuration)
{
    m_configuration = configuration;
}
