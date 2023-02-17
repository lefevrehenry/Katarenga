// Katarenga
#include <client/application.hpp>
#include <client/client.hpp>
#include <client/client_utils.hpp>

#include <common/common_utils.hpp>

int main()
{
    zmqpp::context context;

    {
        Initialize();
    }

    ServerInfo server_info = ReadConfigFile();
    server_info.context = &context;

    AppInfo app_info;
    app_info.context = &context;
    app_info.thread_endpoint = "inproc://katarenga_client";

    Client client(server_info, app_info);

    return 0; //client.exec();
}
