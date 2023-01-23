#include "client_registry.hpp"

// Katarenga
#include <server/server.hpp>

ClientRegistry::ClientRegistry() :
    m_clients()
{
}

bool ClientRegistry::client_exists(ClientId id) const
{
    return m_clients.find(id) != m_clients.end();
}

bool ClientRegistry::add_client(ClientId id, const ClientSocket::SPtr& socket)
{
    if(client_exists(id) || !socket)
        return false;

    if(m_clients.size() >= MAX_CLIENT)
        return false;

    m_clients.insert(std::make_pair(id, socket));

    client_added(id);

    return true;
}

bool ClientRegistry::remove_client(ClientId id)
{
    if(!client_exists(id))
        return false;

    m_clients.erase(id);

    client_removed(id);

    return true;
}

ClientRegistry::ClientSocket::SPtr ClientRegistry::socket(ClientId id) const
{
    if(!client_exists(id))
        throw std::runtime_error("client doesn't exist");

    return m_clients.at(id);
}
