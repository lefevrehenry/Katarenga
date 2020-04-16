#include "ReplyConnectivity.hpp"

ReplyConnectivity::ReplyConnectivity() : MessageWrapper(),
    m_accepted(false)
{

}

ReplyConnectivity::ReplyConnectivity(bool accepted) : MessageWrapper(),
    m_accepted(accepted)
{

}

void ReplyConnectivity::toMessage(zmqpp::message& message)
{
    message << m_accepted;
}

void ReplyConnectivity::fromMessage(zmqpp::message& message)
{
    message >> m_accepted;
}

bool ReplyConnectivity::getAccepted() const
{
    return m_accepted;
}

void ReplyConnectivity::setAccepted(bool accepted)
{
    m_accepted = accepted;
}
