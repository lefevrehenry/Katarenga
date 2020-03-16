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

    StopGame(zmqpp::message& message);
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getReason() const;
    void setReason(const std::string& move);

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

    GameStopped(zmqpp::message& message);
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getReason() const;
    void setReason(const std::string& reason);

private:
    std::string m_reason;

};

#endif
