#ifndef KATARENGA_CLIENT_SOCKETS_WORKER_SOCKET_HPP
#define KATARENGA_CLIENT_SOCKETS_WORKER_SOCKET_HPP

// Katarenga
#include <common/sockets/pair_socket.hpp>

// Standard Library
#include <memory>

class Application;

/**
 * @brief The WorkerSocket class
 */
class WorkerSocket : public PairSocket<WorkerSocket>
{

public:
    using SPtr = std::shared_ptr<WorkerSocket>;

public:
    WorkerSocket(Application* application, zmqpp::context* context, const zmqpp::endpoint_t& endpoint);

public:
    template< typename M >
    void execute_receive_message(const typename M::Parameters& parameters) {
        throw std::runtime_error("No implementation yet");
    }

    template< typename M >
    typename M::Parameters execute_send_message() {
        throw std::runtime_error("No implementation yet");
    }

private:
    Application* m_application;

};

#endif // KATARENGA_CLIENT_SOCKETS_WORKER_SOCKET_HPP
