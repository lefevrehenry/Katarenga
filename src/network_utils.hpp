#ifndef NETWORK_UTILS_HPP
#define NETWORK_UTILS_HPP

#include "zmq.hpp"
#include <string>

//  Receive message from socket and convert into string
std::string s_recv (zmq::socket_t & socket);

//  Convert string to message and send to socket
bool s_send (zmq::socket_t & socket, std::string & string);
bool s_send(zmq::socket_t & socket, const char * cstring);

#endif // NETWORK_UTILS_HPP

