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
    if (verbose)
        std::cout << "[render thread]" << msg << std::endl;
}

void player_msg(const std::string& msg)
{
    if (verbose)
        std::cout << "[player thread]" << msg << std::endl;
}

void convert_move_str(const std::string& move_str, int& source, int& dest)
{
    std::string src_str = move_str.substr(1,2);
    std::string dest_str = move_str.substr(4,2);
    source = stoi(src_str);
    dest = stoi(dest_str);
}

