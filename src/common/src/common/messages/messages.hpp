#ifndef KATARENGA_COMMON_MESSAGES_MESSAGES_HPP
#define KATARENGA_COMMON_MESSAGES_MESSAGES_HPP

// Katrenga
#include <common/common_utils.hpp>

/**
 * Requet/Reply form
 *   - setup connecion with the server
 *   - ping it
 */
struct Ping
{
    struct Request {
        struct Parameters {
        };
    };

    struct Reply {
        struct Parameters {
        };
    };
};

struct NewConnection
{
    struct Request {
        struct Parameters {
            char name[32];
            char ip[32];
            char port[32];
        };
    };

    struct Reply {
        struct Parameters {
            bool accepted;
            char pair_endpoint[128];
        };
    };
};

/**
 * One way message form
 *   - close connection with the server
 *   - setup a game (create, join or spectate)
 *   - play a move
 */
struct CloseConnection {
    struct Parameters {
    };
};

struct CreateGame {
    struct Parameters {
        Common::GameActor actor;
    };
};

struct GameCreated {
    struct Parameters {
        bool accepted;
        Common::GameId id;
        Common::GameActor actor;
        char position[160];
    };
};

struct JoinGame {
    struct Parameters {
        Common::GameId id;
    };
};

struct GameJoined {
    struct Parameters {
        bool accepted;
        Common::GameId id;
        Common::GameActor actor;
        char position[160];
    };
};

struct SpectateGame {
    struct Parameters {
        Common::GameId id;
    };
};

struct GameSpectated {
    struct Parameters {
        bool accepted;
    };
};

struct PlayMove
{
    struct Request {
        struct Parameters {
            Common::Move move;
        };
    };

    struct Reply {
        struct Parameters {
            bool accepted;
            Common::Move move;
        };
    };
};

#endif // KATARENGA_COMMON_MESSAGES_MESSAGES_HPP
