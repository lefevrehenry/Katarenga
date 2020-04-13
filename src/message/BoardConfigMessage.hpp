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

public:
    GameInit();
    GameInit(const std::string& board_config, int current_player);

public:
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getConfiguration() const;
    void setConfiguration(const std::string& configuration);

    int getCurrentPlayer() const;
    void setCurrentPlayer(int player);

    //int getSelfPlayer() const;
    //void setSelfPlayer(int player);

private:
    std::string m_configuration;
    int m_current_player;
    //int m_self_player;
};

class AskBoardConfiguration : MessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::AskBoardConfiguration;
    }

public:
    AskBoardConfiguration();
    AskBoardConfiguration(int player);

public:
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    int getPlayer() const;
    void setPlayer(int player);

private:
    int m_player;

};

class AnswerBoardConfiguration : MessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::AnswerBoardConfiguration;
    }

public:
    AnswerBoardConfiguration();
    AnswerBoardConfiguration(const std::string& configuration) : m_configuration(configuration) {}

public:
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getConfiguration() const;
    void setConfiguration(const std::string& configuration);

private:
    std::string m_configuration;

};

#endif
