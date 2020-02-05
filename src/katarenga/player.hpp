#ifndef PLAYER_HPP
#define PLAYER_HPP

// Standard Library
#include <string>

namespace zmqpp {
    class context;
}

struct PlayerInfo {
    zmqpp::context* context;

    const std::string server_binding_point = "katarenga-server-thread";
    const std::string render_binding_point = "katarenga-render-thread";

};

extern PlayerInfo PlayerInfo;

/* function executed in its own thread */
void player_function();


#endif // PLAYER_HPP

