#include "server_utils.hpp"

// Standard Library
#include <iostream>

static const std::string BLACK         = std::string("\033[0;30m");
static const std::string RED           = std::string("\033[0;31m");
static const std::string GREEN         = std::string("\033[0;32m");
static const std::string YELLOW        = std::string("\033[0;33m");
static const std::string BLUE          = std::string("\033[0;34m");
static const std::string PURPLE        = std::string("\033[0;35m");
static const std::string CYAN          = std::string("\033[0;36m");
static const std::string WHITE         = std::string("\033[0;37m");
static const std::string DEFAULT_COLOR = std::string("\033[0m");

void msg_server(const std::string& msg)
{
//    static const std::string white = "\033[0m";
//    static const std::string red = "\033[31m";

    std::cout << RED << "[server]" << DEFAULT_COLOR << " " << msg << std::endl;
}

void msg_client(const std::string& msg)
{
//    static const std::string white = "\033[0m";
//    static const std::string green = "\033[34m";

    std::cout << GREEN << "[client]" << DEFAULT_COLOR << " " << msg << std::endl;
}
