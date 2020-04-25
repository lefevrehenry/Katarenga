#ifndef SHARED_UTILS_HPP
#define SHARED_UTILS_HPP

#include <string>

enum class GameActor
{
     None
    ,Server
    ,WhiteClient
    ,BlackClient
    ,Render
    //,Spectator
};

std::string to_string(const GameActor& actor);

#endif
