#ifndef PLAYTHISMOVE_HPP
#define PLAYTHISMOVE_HPP

#include "MessageWrapper.hpp"
//#include "../server/utils.hpp"

// Standard Library
#include <string>


/**
 * @brief The PlayThisMove struct
 */
struct PlayThisMove
{
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::PlayThisMove;
    }

    class Request : public MessageWrapper
    {

    public:
        Request();

    public:
        void toMessage(zmqpp::message& message) override;
        void fromMessage(zmqpp::message& message) override;

    public:
        std::string getMove() const;
        void setMove(const std::string& move);

    private:
        std::string m_move;

    };

    class Reply : public MessageWrapper
    {

    public:
        Reply();

    public:
        void toMessage(zmqpp::message& message) override;
        void fromMessage(zmqpp::message& message) override;

    public:
        bool isAccepted() const;
        void setAccepted(bool accepted);

    private:
        bool m_isAccepted;

    };

    class Broadcast : public MessageWrapper {

    public:
        Broadcast();

    public:
        void toMessage(zmqpp::message& message) override;
        void fromMessage(zmqpp::message& message) override;

    public:
        std::string getMove() const;
        void setMove(const std::string& move);

    private:
        std::string m_move;

    };
};

#endif
