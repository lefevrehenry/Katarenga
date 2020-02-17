#ifndef BOARDCONFIGMESSAGE_HPP
#define BOARDCONFIGMESSAGE_HPP

#include "MessageWrapper.hpp"

// Standard Library
#include <string>


class AskBoardConfiguration : DefaultMessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::AskBoardConfiguration;
    }
};

class AnswerBoardConfiguration : MessageWrapper
{
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::AnswerBoardConfiguration;
    }

public:
    AnswerBoardConfiguration(zmqpp::message& message);

    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getConfiguration() const;
    void setConfiguration(const std::string& configuration);

private:
    std::string m_configuration;

};

#endif
