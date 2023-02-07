#ifndef KATARENGA_SERVER_CLIENT_REGISTRY_HPP
#define KATARENGA_SERVER_CLIENT_REGISTRY_HPP

// Katarenga
#include <server/sockets/player_socket.hpp>

// Nod
#include <nod/nod.hpp>

// Standard Library
#include <map>
#include <memory>

/**
 * @brief The ClientRegistry class
 */
class ClientRegistry
{
public:
    using ClientId = std::string;
    using ClientSocket = PlayerSocket;

    using Clients = std::map<ClientId, ClientSocket::SPtr>;

    static ClientId Id(const std::string& ip, const std::string& port)
    {
        return "client(" + ip + ":" + port + ")";
    }

public:
    ClientRegistry();

    ClientRegistry(const ClientRegistry&) = delete;
    ClientRegistry& operator=(const ClientRegistry& other) = delete;

    ClientRegistry(ClientRegistry&&) = delete;
    ClientRegistry& operator=(ClientRegistry&& other) = delete;

public:
    bool client_exists(ClientId id) const;

    bool add_client(ClientId id, const ClientSocket::SPtr& socket);
    bool remove_client(ClientId id);

public:
    ClientSocket::SPtr socket(ClientId id) const;
    ClientId id(const ClientSocket::SPtr& socket) const;

public:
    nod::signal<void(ClientId)> client_added;
    nod::signal<void(ClientId)> client_removed;

private:
    Clients m_clients;

};

#endif // KATARENGA_SERVER_CLIENT_REGISTRY_HPP
