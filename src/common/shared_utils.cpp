#include "shared_utils.hpp"


std::string to_string(const GameActor& actor)
{
    std::string s;
    switch(actor)
    {
    case GameActor::Server:
        s = "Server";
        break;
    case GameActor::WhiteClient:
        s = "White";
        break;
    case GameActor::BlackClient:
        s = "Black";
        break;
    case GameActor::Render:
        s = "Graphics";
    default:
        s = "Unknown";
    }
    return s;
}
