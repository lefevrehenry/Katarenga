#include "utils.hpp"

#include <iostream>

/* Network-related function */
//  Receive message from socket and convert into string
std::string s_recv(zmqpp::socket& socket)
{
    zmqpp::message message;
    socket.receive(message);

    // how to handle case where the message received has multiple parts ?

    std::string result;
    message >> result;

    return result;
}

//  Convert string to message and send to socket
/*bool s_send(zmqpp::socket& socket, const std::string& string)
{
    zmqpp::message message;
    message << string;

    return socket.send(message);
}

bool s_send(zmqpp::socket& socket, const char* cstring)
{
    std::string s = cstring;
    return s_send(socket, s);
}*/



/* Misc functions */
void render_msg(const std::string& msg)
{
    static const std::string white = "\033[0m";
    static const std::string yellow = "\033[33m";
    if (verbose)
        std::cout << yellow << "[render thread]" << white << " " << msg << std::endl;
}

void player_msg(const std::string& msg)
{
    static const std::string white = "\033[0m";
    static const std::string green = "\033[32m";

    if (verbose)
        std::cout << green << "[player thread]" << white << " " << msg << std::endl;
}

void convert_move_str(const std::string& move_str, int& source, int& dest)
{
    std::string src_str = move_str.substr(1,2);
    std::string dest_str = move_str.substr(4,2);
    source = stoi(src_str);
    dest = stoi(dest_str);
}

std::string create_move_str(int source, int dest)
{
    return std::to_string(source) + ":" + std::to_string(dest);
}

