#ifndef QUITMESSAGE_HPP
#define QUITMESSAGE_HPP

#include "MessageWrapper.hpp"
//#include "../server/utils.hpp"

// Standard Library
#include <string>


class StopGame : MessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::StopGame;
    }

public:
    StopGame();
    StopGame(const std::string& reason);

public:
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getReason() const;
    void setReason(const std::string& reason);

private:
    std::string m_reason;

};

class GameStopped : MessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::GameStopped;
    }

public:
    GameStopped();
    GameStopped(const std::string& reason);

public:
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getReason() const;
    void setReason(const std::string& reason);

private:
    std::string m_reason;

};

#endif
