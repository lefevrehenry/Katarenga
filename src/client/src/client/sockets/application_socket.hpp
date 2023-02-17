#ifndef KATARENGA_CLIENT_SOCKETS_APPLICATION_SOCKET_HPP
#define KATARENGA_CLIENT_SOCKETS_APPLICATION_SOCKET_HPP

// Katarenga
#include <common/sockets/pair_socket.hpp>

// Standard Library
#include <memory>

class Client;

/**
 * @brief The ApplicationSocket class
 */
class ApplicationSocket : public PairSocket<ApplicationSocket>
{

public:
    using SPtr = std::shared_ptr<ApplicationSocket>;

public:
    ApplicationSocket(Client* client, zmqpp::context* context, const zmqpp::endpoint_t& endpoint);

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
    Client* m_client;

};

#endif // KATARENGA_CLIENT_SOCKETS_APPLICATION_SOCKET_HPP
