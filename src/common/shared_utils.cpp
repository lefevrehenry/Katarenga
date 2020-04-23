#include "shared_utils.hpp"


std::string to_string(const GameActor& actor)
{
    std::string s;
    switch(actor)
    {
    case GameActor::SERVER:
        s = "Server";
        break;
    case GameActor::WHITECLIENT:
        s = "White";
        break;
    case GameActor::BLACKCLIENT:
        s = "Black";
        break;
    case GameActor::RENDER:
        s = "Graphics";
    default:
        s = "Unknown";
    }
    return s;
}
