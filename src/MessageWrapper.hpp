#ifndef MESSAGE_HPP
#define MESSAGE_HPP

// ZMQ
#include <zmqpp/message.hpp>

#include <string>

struct MessageWrapper
{   
    enum class MessageType {
        AskBoardConfiguration,
        CheckConnectivity,
        IsThisMoveValid,
        ForgetItRageQuit
    };

public:
    virtual void toMessage(zmqpp::message& message) = 0;
    virtual void fromMessage(zmqpp::message& message) = 0;

};

///\brief Provide a default implementation of a MessageWrapper with empty implementations of pure virtuals methods
struct DefaultMessageWrapper : public MessageWrapper
{
    virtual void toMessage(zmqpp::message&) override {}
    virtual void fromMessage(zmqpp::message&) override {}
};

struct BoardConfiguration
{
    static MessageWrapper::MessageType MessageType();

    class Request : public DefaultMessageWrapper
    {
    };

    class Reply : public MessageWrapper
    {

    public:
        Reply();

    public:
        void toMessage(zmqpp::message& message) override;
        void fromMessage(zmqpp::message& message) override;

    public:
        std::string getConfiguration() const;
        void setConfiguration(const std::string& configuration);

    private:
        std::string m_configuration;

    };
};

struct CheckConnectivity
{
    static MessageWrapper::MessageType MessageType();

    class Request : public DefaultMessageWrapper
    {
    };

    class Reply : public MessageWrapper
    {

    public:
        Reply();

    public:
        virtual void toMessage(zmqpp::message& message);
        virtual void fromMessage(zmqpp::message& message);

    public:
        bool getConnectivity() const;
        void setConnectivity(bool connectivity);

    private:
        bool m_connectivity;

    };
};

#endif
