#include "client_registry.hpp"

ClientRegistry::ClientRegistry() :
    m_client_sockets()
{
}

bool ClientRegistry::client_exists(ClientId id) const
{
    return m_client_sockets.find(id) != m_client_sockets.end();
}

bool ClientRegistry::add_client(ClientId id, const ClientSocket::SPtr& socket)
{
    if(client_exists(id) || !socket)
        return false;

    m_client_sockets.insert(std::make_pair(id, socket));

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

ClientRegistry::ClientSocket::SPtr ClientRegistry::socket(ClientId id) const
{
    if(!client_exists(id))
        throw std::runtime_error("client doesn't exist");

    return m_client_sockets.at(id);
}
