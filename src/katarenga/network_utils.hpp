#ifndef NETWORK_UTILS_HPP
#define NETWORK_UTILS_HPP

#include <zmqpp/zmqpp.hpp>
#include <string>

//  Receive message from socket and convert into string
std::string s_recv(zmqpp::socket& socket);

//  Convert string to message and send to socket
bool s_send(zmqpp::socket& socket, const std::string& string);
bool s_send(zmqpp::socket& socket, const char* cstring);

#endif // NETWORK_UTILS_HPP

