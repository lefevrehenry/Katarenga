#ifndef KATARENGA_CLIENT_SOCKETS_SERVER_SOCKET_HPP
#define KATARENGA_CLIENT_SOCKETS_SERVER_SOCKET_HPP

// Katarenga
#include <common/common_utils.hpp>
#include <common/sockets/pair_socket.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <memory>

class Client;

/**
 * @brief The ServerSocket class
 */
class ServerSocket : public PairSocket<ServerSocket>, public std::enable_shared_from_this<ServerSocket>
{
    using GameActor = Common::GameActor;

public:
    using SPtr = std::shared_ptr<ServerSocket>;

public:
    ServerSocket(Client* client, zmqpp::context* context, const zmqpp::endpoint_t& endpoint);

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

#endif // KATARENGA_CLIENT_SOCKETS_SERVER_SOCKET_HPP
