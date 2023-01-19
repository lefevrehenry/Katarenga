#include "client_registry.hpp"

ClientRegistry::ClientRegistry() :
    m_client_sockets()
{
}

bool ClientRegistry::client_exists(ClientId id) const
{
    return m_client_sockets.find(id) != m_client_sockets.end();
}

bool ClientRegistry::has_input(zmqpp::poller* poller) const
{
    ClientSockets::const_iterator it;

    for (it = m_client_sockets.begin(); it != m_client_sockets.end(); ++it)
    {
        ClientId id = it->first;
        ClientSocket socket = it->second;

        if(poller->has_input(*socket.get()))
            return true;
    }

    return false;
}

bool ClientRegistry::add_client(ClientId id, const ClientSocket& socket)
{
    if(client_exists(id))
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

ClientRegistry::ClientSocket ClientRegistry::socket(ClientId id) const
{
    if(!client_exists(id))
        throw std::runtime_error("client doesn't exists");

    return m_client_sockets.at(id);
}
