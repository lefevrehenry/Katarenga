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
public:
    using ClientId = std::string;
    using ClientSocket = std::shared_ptr<PlayerSocket>;

    using ClientSockets = std::map<ClientId, ClientSocket>;

    static ClientId Id(const std::string& ip, const std::string& port)
    {
        return "client(" + ip + ":" + port + ")";
    }

public:
    ClientRegistry();

    ClientRegistry(const ClientRegistry&) = delete;
    ClientRegistry& operator=(const ClientRegistry& other) = delete;

public:
    bool client_exists(ClientId id);

    ClientSocket add_client(ClientId id);
    bool remove_client(ClientId id);

private:
    ClientSockets m_client_sockets;

};

#endif // KATARENGA_SERVER_CLIENT_REGISTRY_HPP
