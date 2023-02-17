#include "worker_socket.hpp"

// Katarenga
#include <client/application.hpp>
#include <client/messages/client_messages.hpp>

WorkerSocket::WorkerSocket(Application* application, zmqpp::context* context, const zmqpp::endpoint_t& endpoint) :
    PairSocket(context, endpoint),
    m_application(application)
{
    connect(endpoint);

    registerSendMessage<Quit>();
}

template<>
typename Quit::Parameters WorkerSocket::execute_send_message<Quit>()
{
    return {};
}
