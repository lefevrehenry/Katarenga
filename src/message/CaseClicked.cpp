#include "CaseClicked.hpp"

CaseClicked::CaseClicked(const std::string& caseClicked) :
    m_case(caseClicked)
{

}

CaseClicked::CaseClicked(zmqpp::message& message)
{
    fromMessage(message);
}

void CaseClicked::toMessage(zmqpp::message& message)
{
    message << m_case;
}

void CaseClicked::fromMessage(zmqpp::message& message)
{
    message >> m_case;
}

std::string CaseClicked::getCase() const
{
    return m_case;
}

void CaseClicked::setCase(const std::string& caseClicked)
{
    m_case = caseClicked;
}
