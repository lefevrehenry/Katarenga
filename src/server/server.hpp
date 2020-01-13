#ifndef SERVER_HPP
#define SERVER_HPP

struct ServerInfo
{
    int server_white_port;          // The server socket port for the white player
    int server_black_port;          // The server socket port for the black player
    bool verbose;                   // Whether to be verbose or not
};

extern ServerInfo ServerInfo;

#endif // SERVER_HPP

