#ifndef KATARENGA_COMMON_MESSAGES_MESSAGES_HPP
#define KATARENGA_COMMON_MESSAGES_MESSAGES_HPP

// Katrenga
#include <common/common_utils.hpp>

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

#endif // KATARENGA_COMMON_MESSAGES_MESSAGES_HPP
