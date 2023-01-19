#ifndef KATARENGA_SERVER_CLIENT_REGISTRY_HPP
#define KATARENGA_SERVER_CLIENT_REGISTRY_HPP

// Katarenga
#include <server/sockets/player_socket.hpp>

// Nod
#include <nod/nod.hpp>

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
    bool client_exists(ClientId id) const;

    bool has_input(zmqpp::poller* poller) const;

    bool add_client(ClientId id, const ClientSocket& socket);
    bool remove_client(ClientId id);

public:
    ClientSocket socket(ClientId id) const;

public:
    nod::signal<void(ClientId)> client_added;
    nod::signal<void(ClientId)> client_removed;

private:
    ClientSockets m_client_sockets;

};

#endif // KATARENGA_SERVER_CLIENT_REGISTRY_HPP
