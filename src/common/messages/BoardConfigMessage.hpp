#ifndef BOARDCONFIGMESSAGE_HPP
#define BOARDCONFIGMESSAGE_HPP

#include "MessageWrapper.hpp"
#include "../board/board_utils.hpp"

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
    GameInit(const std::string& board_config, BoardPlayer current_player);

public:
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getConfiguration() const;
    void setConfiguration(const std::string& configuration);

    BoardPlayer getCurrentPlayer() const;
    void setCurrentPlayer(BoardPlayer player);

private:
    std::string m_configuration;
    BoardPlayer m_current_player;
};

class AskBoardConfiguration : public DefaultMessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::AskBoardConfiguration;
    }

/*    AskBoardConfiguration();

    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;*/
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
