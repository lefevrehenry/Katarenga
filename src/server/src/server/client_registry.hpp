#ifndef KATARENGA_SERVER_CLIENT_REGISTRY_HPP
#define KATARENGA_SERVER_CLIENT_REGISTRY_HPP

// Katarenga
#include <server/sockets/player_socket.hpp>

// Standard Library
#include <memory>

/**
 * @brief The ClientRegistry class
 */
class ClientRegistry
{
    using ClientId = unsigned int;
    using ClientSocket = std::shared_ptr<PlayerSocket>;

    using ClientSockets = std::map<ClientId, ClientSocket>;

public:
    ClientRegistry();

    ClientRegistry(const ClientRegistry&) = delete;
    ClientRegistry& operator=(const ClientRegistry& other) = delete;

public:
    bool client_exists(ClientId id);

    bool add_client(ClientId id);
    bool remove_client(ClientId id);

private:
    ClientSockets m_client_sockets;

};

#endif // KATARENGA_SERVER_CLIENT_REGISTRY_HPP
