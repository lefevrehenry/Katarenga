#include "MessageWrapper.hpp"

MessageWrapper::MessageType BoardConfiguration::MessageType()
{
    return MessageWrapper::MessageType::AskBoardConfiguration;
}

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

MessageWrapper::MessageType CheckConnectivity::MessageType()
{
    return MessageWrapper::MessageType::CheckConnectivity;
}

CheckConnectivity::Reply::Reply() : MessageWrapper(),
    m_connectivity(false)
{
}

void CheckConnectivity::Reply::toMessage(zmqpp::message& message)
{
    message << m_connectivity;
}

void CheckConnectivity::Reply::fromMessage(zmqpp::message& message)
{
    message >> m_connectivity;
}

bool CheckConnectivity::Reply::getConnectivity() const
{
    return m_connectivity;
}

void CheckConnectivity::Reply::setConnectivity(bool connectivity)
{
    m_connectivity = connectivity;
}
