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

    StopGame(const std::string& reason, const int player) : m_reason(reason), m_player(player) {}
    StopGame(zmqpp::message& message);
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getReason() const;
    void setReason(const std::string& reason);

    int getPlayer() const;
    void setPlayer(const int player);

private:
    std::string m_reason;
    int m_player;

};

class GameStopped : MessageWrapper
{
public:
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::GameStopped;
    }

    GameStopped(const std::string& reason) : m_reason(reason) {}
    GameStopped(zmqpp::message& message);
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getReason() const;
    void setReason(const std::string& reason);

private:
    std::string m_reason;

};

#endif
