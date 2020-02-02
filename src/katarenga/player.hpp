#ifndef PLAYER_HPP
#define PLAYER_HPP

// Standard Library
#include <string>

namespace zmqpp {
    class context;
}

struct PlayerInfo {
    zmqpp::context* context;

    const std::string server_communication_binding_point = "server_communication";
    const std::string render_binding_point = "render_thread";

};

extern PlayerInfo PlayerInfo;

/* function executed in its own thread */
void player_function();


#endif // PLAYER_HPP

