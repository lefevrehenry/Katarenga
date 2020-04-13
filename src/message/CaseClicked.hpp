#ifndef CASECLICKED_HPP
#define CASECLICKED_HPP

#include "MessageWrapper.hpp"

// Standard Library
#include <string>


class CaseClicked : MessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::CaseClicked;
    }

    CaseClicked(const std::string& caseClicked);
    CaseClicked(zmqpp::message& message);

public:
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getCase() const;
    void setCase(const std::string& caseClicked);

private:
    std::string m_case;

};

#endif
