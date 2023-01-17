#include "server_utils.hpp"

// Standard Library
#include <iostream>

void msg_server(const std::string& msg)
{
    static const std::string white = "\033[0m";
    static const std::string red = "\033[31m";

    std::cout << red << "[server]" << white << " " << msg << std::endl;
}
