#ifndef BOARDCONFIGURATION_HPP
#define BOARDCONFIGURATION_HPP

#include "MessageWrapper.hpp"

// Standard Library
#include <string>


/**
 * @brief The BoardConfiguration struct
 */
struct BoardConfiguration
{
    static MessageWrapper::MessageType MessageType()
    {
        return MessageWrapper::MessageType::AskBoardConfiguration;
    }

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

    class Broadcast : public DefaultMessageWrapper
    {
    };
};

#endif
