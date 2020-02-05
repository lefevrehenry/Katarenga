#ifndef MOVEMESSAGE_HPP
#define MOVEMESSAGE_HPP

#include "MessageWrapper.hpp"
//#include "../server/utils.hpp"

// Standard Library
#include <string>


class PlayThisMove : MessageWrapper
{
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::PlayThisMove;
    }

public:
    PlayThisMove(std::string& move);
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getMove() const;
    void setMove(const std::string& move);

private:
    std::string m_move;

};

class MovePlayed : MessageWrapper
{
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::MovePlayed;
    }

public:
    MovePlayed(std::string& move);
    void toMessage(zmqpp::message& message) override;
    void fromMessage(zmqpp::message& message) override;

    std::string getMove() const;
    void setMove(const std::string& move);

private:
    std::string m_move;

};

#endif
