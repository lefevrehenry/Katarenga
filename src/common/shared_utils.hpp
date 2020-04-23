#ifndef SHARED_UTILS_HPP
#define SHARED_UTILS_HPP

#include <string>

enum class GameActor
{
     NONE
    ,SERVER
    ,WHITECLIENT
    ,BLACKCLIENT
    ,RENDER
    //,SPECTATOR
};

std::string to_string(const GameActor& actor);

#endif
