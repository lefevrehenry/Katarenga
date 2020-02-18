#ifndef PLAYER_HPP
#define PLAYER_HPP

// Standard Library
#include <string>

namespace zmqpp {
    class context;
    class poller;
}

struct PlayerInfo {
    zmqpp::context* zmq_context;
    int self_player; // 1 for White, -1 for Black
    int current_player; // Which turn is it
    bool game_finished;
    const std::string render_binding_point = "inproc://katarenga-render-thread";

};

extern PlayerInfo PlayerInfo;

/* function executed in its own thread */
void player_function();

void init_sockets_and_poller(zmqpp::poller& poller);


#endif // PLAYER_HPP

