// Katarenga
#include <server/server.hpp>
#include <server/server_utils.hpp>

// docopt
//#include <docopt/docopt.h>

// Standard Library
#include <iostream>
#include <map>
#include <string>
#include <filesystem>

// Ini
#include <ini/ini.h>

//static int parse_arguments(int argc, char * argv[], ServerInfo &server_info)
//{
//    static const char usage[] =
//R"(Katarenga-Server: A nice two-player board game!

//Usage:
//    server                      [options]
//    server -h | --help

//Input options:
//    --offset-port <port>        The offset on which socket ports will be based on.
//                                In the default case the white port will be 28000 and black port 28001.
//                                [default: 28000]

//Other options:
//    -v, --verbose                  Makes katarenga verbose.
//    -h, --help                     Shows this help.
//)";

//    std::map<std::string, docopt::value> args = docopt::docopt(usage, {argv+1, argv+argc}, true);

//    int offset_port = args["--offset-port"].asLong();
//    bool verbose = args["--verbose"].asBool();

//    server_info.server_white_port = offset_port;
//    server_info.server_black_port = offset_port + 1;
//    //server_info.server_publish_port = offset_port + 2;

//    server_info.white_binding_point = "tcp://*:" + std::to_string(server_info.server_white_port);
//    server_info.black_binding_point = "tcp://*:" + std::to_string(server_info.server_black_port);

//    server_info.verbose = verbose;

//    return 0;
//}

namespace fs = std::filesystem;

ServerInfo ReadConfigFile()
{
    std::string home = std::getenv("HOME");

    fs::path config_directory = fs::path(home) / ".config" / "katarenga";
    fs::path config_file = config_directory / "server.cfg";

    inih::INIReader ini(config_file);

    // Get and parse the ini value
    std::string thread_port = ini.Get<std::string>("thread", "port");
    std::string processus_port = ini.Get<std::string>("processus", "port");
    std::string tcp_port = ini.Get<std::string>("tcp", "port");

    ServerInfo config;
    config.thread_endpoint = "inproc://" + thread_port;
    config.processus_endpoint = "ipc://" + processus_port;
    config.network_endpoint = "tcp://*:" + tcp_port;

    return config;
}

void client(const ServerInfo& server_info)
{
    zmqpp::context context;

    zmqpp::socket socket_push(context, zmqpp::socket_type::request);
    socket_push.connect(server_info.processus_endpoint);

    zmqpp::poller poller;
    poller.add(socket_push, zmqpp::poller::poll_in);

    NewConnection::Request::Parameters R = {"jacky", "0.0.0.1", "28000"};
    zmqpp::message message1 = Message::Create<NewConnection::Request>(R);
//    zmqpp::message message2 = Message::Create<GameStatus>();
//    zmqpp::message message3 = Message::Create<MovePlayer>(50);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    msg_server("send 1");
    socket_push.send(message1);

    if(poller.poll(5000))
    {
        if(poller.has_input(socket_push))
        {
            zmqpp::message reply_message;
            socket_push.receive(reply_message);

            typename NewConnection::Reply::Parameters reply = Message::Payload<NewConnection::Reply>(reply_message);

            msg_server("msg received from server '" + std::to_string(reply.accepted) + "'");
            msg_server("msg received from server '" + std::string(reply.pair_endpoint) + "'");
        }
    }
    else
    {
        msg_server("server not responding");
    }
}

int main(int argc, char* argv[])
{
    // Let's parse the command-line arguments!
//    ServerInfo server_info;
//    if (parse_arguments(argc, argv, server_info))
//        return 1;

    Message::Id<NewConnection>();

    ServerInfo server_info = ReadConfigFile();

    Server server(server_info);

    std::thread t1(client, server_info);

    server.loop();

    t1.join();

    return 0;
}
