#ifndef KATARENGA_COMMON_MESSAGES_MESSAGES_HPP
#define KATARENGA_COMMON_MESSAGES_MESSAGES_HPP

// Katrenga
#include <common/common_utils.hpp>

//struct MovePlayer {
//    struct Parameters {
//        int a;
//    };
//};

//struct GameStatus {
//    struct Parameters {
//    };
//};

struct NewConnection
{
    struct Request {
        struct Parameters {
            const char* name;
            const char* ip;
            const char* port;
        };
    };

    struct Reply {
        struct Parameters {
            bool accepted;
            char pair_endpoint[128];
        };
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

//struct CloseConnection {
//    struct Parameters {
//    };
//};

#endif // KATARENGA_COMMON_MESSAGES_MESSAGES_HPP
