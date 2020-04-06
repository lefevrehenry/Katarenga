#ifndef BOARDCONFIGMESSAGE_HPP
#define BOARDCONFIGMESSAGE_HPP

#include "MessageWrapper.hpp"

// Standard Library
#include <string>

class GameInit : MessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::GameInit;
    }

    GameInit(zmqpp::message& message);

    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getConfiguration() const;
    void setConfiguration(const std::string& configuration);

    int getCurrentPlayer() const;
    void setCurrentPlayer(const int player);

    //int getSelfPlayer() const;
    //void setSelfPlayer(const int player);

private:
    std::string m_configuration;
    int m_current_player;
    //int m_self_player;
};

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
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::AnswerBoardConfiguration;
    }

    AnswerBoardConfiguration(zmqpp::message& message);

    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getConfiguration() const;
    void setConfiguration(const std::string& configuration);

private:
    std::string m_configuration;

};

#endif
