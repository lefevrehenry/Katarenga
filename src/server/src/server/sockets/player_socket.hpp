#ifndef KATARENGA_SERVER_SOCKETS_PLAYER_SOCKET_HPP
#define KATARENGA_SERVER_SOCKETS_PLAYER_SOCKET_HPP

// Katarenga
#include <common/sockets/pair_socket.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <memory>

class Server;

/**
 * @brief The PlayerSocket class
 */
class PlayerSocket : public PairSocket<PlayerSocket>, public std::enable_shared_from_this<PlayerSocket>
{
public:
    using SPtr = std::shared_ptr<PlayerSocket>;

public:
    PlayerSocket(Server* server, zmqpp::context* context, const zmqpp::endpoint_t& endpoint);

public:
    bool busy() const;

public:
    void mark_busy() { m_is_busy = true; }
    void unmark_busy() { m_is_busy = false; }

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
    Server* m_server;
    bool m_is_busy;

};

#endif // KATARENGA_SERVER_SOCKETS_PLAYER_SOCKET_HPP
