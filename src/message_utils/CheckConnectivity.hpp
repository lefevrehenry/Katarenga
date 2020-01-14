#ifndef CHECKCONNECTIVITY_HPP
#define CHECKCONNECTIVITY_HPP

#include "MessageWrapper.hpp"


/**
 * @brief The CheckConnectivity struct
 */
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
