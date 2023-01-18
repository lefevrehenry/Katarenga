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
    using ClientEndpoint = zmqpp::endpoint_t;

    using ClientSockets = std::map<ClientId, ClientSocket>;
    using ClientEndpoints = std::map<ClientId, ClientEndpoint>;

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

    bool add_client(ClientId id);
    bool remove_client(ClientId id);

public:
    ClientSocket socket(ClientId id) const;
    void set_socket(ClientId id, const ClientSocket& socket);

    ClientEndpoint endpoint(ClientId id) const;

public:
    nod::signal<void(ClientId)> client_added;

private:
    void VerifyClient(ClientId id) const;

private:
    ClientSockets m_client_sockets;
    ClientEndpoints m_client_endpoints;

};

#endif // KATARENGA_SERVER_CLIENT_REGISTRY_HPP
