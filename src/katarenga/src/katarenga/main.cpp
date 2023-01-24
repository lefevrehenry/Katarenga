
// Katarenga
#include <common/common_utils.hpp>
#include <katarenga/client.hpp>

int main()
{
    ServerInfo server_info = ReadConfigFile();

    Client client(server_info);

    return client.exec();
}
