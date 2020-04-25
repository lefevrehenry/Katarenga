#ifndef GAMESTATUS_HPP
#define GAMESTATUS_HPP

#include "MessageWrapper.hpp"
#include "../board/board_utils.hpp"

// Standard Library
#include <string>


class AskGameStatus : public DefaultMessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::AskGameStatus;
    }

/*    AskBoardConfiguration();

    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;*/
};



class AnswerGameStatus : MessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::AnswerGameStatus;
    }

public:
    AnswerGameStatus();
    AnswerGameStatus(const std::string& board_config, BoardPlayer current_player);

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


#endif
