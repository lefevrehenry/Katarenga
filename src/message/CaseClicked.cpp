#include "CaseClicked.hpp"

CaseClicked::CaseClicked()
{

}

CaseClicked::CaseClicked(const std::string& caseClicked) :
    m_case(caseClicked)
{

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
