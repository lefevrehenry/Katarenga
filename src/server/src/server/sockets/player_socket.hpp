#ifndef KATARENGA_SERVER_SOCKETS_PLAYER_SOCKET_HPP
#define KATARENGA_SERVER_SOCKETS_PLAYER_SOCKET_HPP

// Katarenga
#include <common/sockets/pair_socket.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

class Server;

/**
 * @brief The PlayerSocket class
 */
class PlayerSocket : public PairSocket<PlayerSocket>, public std::enable_shared_from_this<PlayerSocket>
{
public:
    PlayerSocket(Server* server, zmqpp::context* context, const zmqpp::endpoint_t& endpoint);

public:
    template< typename M >
    void execute_receive_message(const typename M::Parameters& parameters) {
        throw std::runtime_error("No implementation yet");
    }

    template< typename M >
    void execute_send_message(typename M::Parameters* parameters) {
        throw std::runtime_error("No implementation yet");
    }

private:
    Server* m_server;

};

#endif // KATARENGA_SERVER_SOCKETS_PLAYER_SOCKET_HPP
