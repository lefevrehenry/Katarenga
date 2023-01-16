#ifndef KATARENGA_SERVER_SERVER_HPP
#define KATARENGA_SERVER_SERVER_HPP

// Katarenga
#include <common/messages/messages.hpp>
#include <common/shared_utils.hpp>
#include <server/server_utils.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <memory>

class ConnectionSocket;

/**
 * @brief The Server class
 */
class Server
{

public:
    Server(const ServerInfo& server_info);

public:
    void loop();

public:
    template<typename M>
    void execute_message(const typename M::Parameters& parameters) {
        throw std::runtime_error("No implementation yet");
    }

    template<typename M>
    typename M::Reply::Parameters execute_message(const typename M::Request::Parameters& parameters) {
//        throw std::runtime_error("No implementation yet");
        server_msg("No implementation defined for " + Message::Name<M>());

        typename M::Reply::Parameters p;
        p.ok = "not ok";
        return p;
    }

public:
    // Socket-related content
    zmqpp::context      m_zmq_context;
    zmqpp::poller       m_poller;

    std::shared_ptr<ConnectionSocket>    m_connection_socket;

    // Game-related content

};

#endif // KATARENGA_SERVER_SERVER_HPP

