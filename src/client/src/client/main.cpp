// Katarenga
#include <common/common_utils.hpp>

#include <client/client.hpp>

int main()
{
    Initialize();

    ServerInfo server_info = ReadConfigFile();

    Client client(server_info);

    return client.exec();
}