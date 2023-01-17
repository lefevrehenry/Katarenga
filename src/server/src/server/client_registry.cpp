#include "client_registry.hpp"

ClientRegistry::ClientRegistry() :
    m_client_sockets()
{
}

bool ClientRegistry::client_exists(ClientId id)
{
    return m_client_sockets.find(id) != m_client_sockets.end();
}

ClientRegistry::ClientSocket ClientRegistry::add_client(ClientId id)
{
    if(client_exists(id))
        return nullptr;

    ClientSocket socket;
    m_client_sockets.insert(std::make_pair(id, socket));

    return socket;
}

bool ClientRegistry::remove_client(ClientId id)
{
    if(!client_exists(id))
        return false;

    m_client_sockets.erase(id);

    return true;
}
