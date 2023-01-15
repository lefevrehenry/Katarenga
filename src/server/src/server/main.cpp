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
    std::string tcp_port = ini.Get<std::string>("tcp", "port");
    std::string localhost_port = ini.Get<std::string>("localhost", "port");

    ServerInfo config;
    config.tcp_endpoint = "tcp://*:" + tcp_port;
    config.localhost_endpoint = "ipc://" + localhost_port;

    return config;
}

int main(int argc, char* argv[])
{
    // Let's parse the command-line arguments!
//    ServerInfo server_info;
//    if (parse_arguments(argc, argv, server_info))
//        return 1;

    ServerInfo server_info = ReadConfigFile();

    Server server(server_info);
    server.loop();

    return 0;
}
