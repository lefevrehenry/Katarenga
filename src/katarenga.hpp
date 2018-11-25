#ifndef KATARENGA_HPP
#define KATARENGA_HPP

#include <string>

struct MainArguments
{
    bool is_server;                 // Whether this is the server-side or client-side.
    std::string socket_endpoint;    // The socket endpoint to use.
    bool verbose;                   // Whether to be verbose.
};

int parse_main_args(int argc, char * argv[], MainArguments & main_args);





#endif // KATARENGA_HPP
