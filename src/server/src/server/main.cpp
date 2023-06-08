// Katarenga
#include <common/common_utils.hpp>

#include <server/server.hpp>

// docopt
//#include <docopt/docopt.h>

//// Standard Library
//#include <iostream>
//#include <map>
//#include <string>
//#include <filesystem>

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

void client(const ServerInfo& server_info)
{
    zmqpp::context context;

    zmqpp::socket socket_request(context, zmqpp::socket_type::request);
    socket_request.connect(server_info.processus_endpoint);

    zmqpp::socket socket_pair(context, zmqpp::socket_type::pair);
//    socket_request.connect(server_info.processus_endpoint);

    zmqpp::poller poller;
    poller.add(socket_request, zmqpp::poller::poll_in);

    NewConnection::Request::Parameters R = {"jacky", "0.0.0.1", "28000"};
    zmqpp::message message1 = Message::Create<NewConnection::Request>(R);
//    zmqpp::message message2 = Message::Create<GameStatus>();
//    zmqpp::message message3 = Message::Create<MovePlayer>(50);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    msg_client("send 1");
    socket_request.send(message1);

    while(true)
    {
        if(poller.poll(5000))
        {
            if(poller.has_input(socket_request))
            {
                zmqpp::message reply_message;
                socket_request.receive(reply_message);

                typename NewConnection::Reply::Parameters reply = Message::Payload<NewConnection::Reply>(reply_message);

                msg_client("msg received from server '" + std::to_string(reply.accepted) + "'");
                msg_client("msg received from server '" + std::string(reply.pair_endpoint) + "'");

                zmqpp::endpoint_t endpoint = std::string(reply.pair_endpoint);
                msg_client("pair connect");
                socket_pair.connect(endpoint);
                poller.add(socket_pair, zmqpp::poller::poll_in);

                zmqpp::message m = Message::Create<CreateGame>();
                socket_pair.send(m);
            }

            if(poller.has_input(socket_pair))
            {
                msg_client("socket_pair receive a message");

                zmqpp::message input_message;
                socket_pair.receive(input_message);

                const GameCreated::Parameters& p = Message::Payload<GameCreated>(input_message);
                msg_client("game created " + std::to_string(p.id));
            }
        }
        else
        {
            msg_client("server not responding");
            break;
        }
    }

    msg_client("client ending");
}

int main()
{
    // Let's parse the command-line arguments!
//    ServerInfo server_info;
//    if (parse_arguments(argc, argv, server_info))
//        return 1;

    zmqpp::context context;

    {
        Initialize();
    }

    ServerInfo server_info = ReadConfigFile();
    server_info.context = &context;

    Server server(server_info);

//    std::thread t1(client, server_info);

    server.loop();

//    t1.join();

    return 0;
}
