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

    GameInit(const std::string &board_config, int current_player) :
        m_configuration(board_config), m_current_player(current_player) {}
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

class AskBoardConfiguration : MessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::AskBoardConfiguration;
    }

    AskBoardConfiguration();
    AskBoardConfiguration(const int player);
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    int getPlayer() const;
    void setPlayer(const int player);

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

    AnswerBoardConfiguration(const std::string& configuration) : m_configuration(configuration) {}
    AnswerBoardConfiguration(zmqpp::message& message);

    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getConfiguration() const;
    void setConfiguration(const std::string& configuration);

private:
    std::string m_configuration;

};

#endif
