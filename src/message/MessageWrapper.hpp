#ifndef MESSAGEWRAPPER_HPP
#define MESSAGEWRAPPER_HPP

// ZMQ
#include <zmqpp/message.hpp>


/**
 * @brief Base class for serialization/deserialize a zmqpp::message into a specific object
 */
struct MessageWrapper
{   
public:
    MessageWrapper()
    {
    }

    enum class MessageType {    // throw it in a separate class
        AskBoardConfiguration,
        CheckConnectivity,
        IsThisMoveValid,
        PlayThisMove,
        ForgetItRageQuit,
        PrintBoard,
        StopGame
    };

public:
    virtual void toMessage(zmqpp::message& message) = 0;
    virtual void fromMessage(zmqpp::message& message) = 0;

};

/**
 * @brief Provide a default implementation of a MessageWrapper with empty implementations of pure virtuals methods
 */
struct DefaultMessageWrapper : public MessageWrapper
{
    virtual void toMessage(zmqpp::message&) override {}
    virtual void fromMessage(zmqpp::message&) override {}
};

#endif
