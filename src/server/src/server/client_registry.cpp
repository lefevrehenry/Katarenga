#include "client_registry.hpp"

ClientRegistry::ClientRegistry() :
    m_client_sockets(),
    m_client_endpoints()
{
}

bool ClientRegistry::client_exists(ClientId id) const
{
    return m_client_sockets.find(id) != m_client_sockets.end();
}

bool ClientRegistry::add_client(ClientId id)
{
    if(client_exists(id))
        return false;

    m_client_sockets.insert(std::make_pair(id, ClientSocket()));
    m_client_endpoints.insert(std::make_pair(id, ""));

    client_added(id);

    return true;
}

bool ClientRegistry::remove_client(ClientId id)
{
    if(!client_exists(id))
        return false;

    m_client_sockets.erase(id);

    return true;
}

ClientRegistry::ClientSocket ClientRegistry::socket(ClientId id) const
{
    VerifyClient(id);

    return m_client_sockets.at(id);
}

void ClientRegistry::set_socket(ClientId id, const ClientSocket& socket)
{
    VerifyClient(id);

    m_client_sockets[id] = socket;
}

ClientRegistry::ClientEndpoint ClientRegistry::endpoint(ClientId id) const
{
    VerifyClient(id);

    return m_client_endpoints.at(id);
}

void ClientRegistry::VerifyClient(ClientId id) const
{
    if(!client_exists(id))
        throw std::runtime_error("client doesn't exists");
}
