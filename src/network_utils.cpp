#include "network_utils.hpp"

//  Receive message from socket and convert into string
std::string s_recv (zmq::socket_t & socket)
{
    zmq::message_t message;
    socket.recv(&message);

    return std::string(static_cast<char*>(message.data()), message.size());
}

//  Convert string to message and send to socket
bool s_send (zmq::socket_t & socket, std::string & string)
{
    zmq::message_t message(string.size());
    memcpy (message.data(), string.data(), string.size());

    return socket.send (message);
}
