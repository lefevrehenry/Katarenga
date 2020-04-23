#include "server_utils.hpp"

#include <common/messages/message_utils.hpp>

#include <iostream>

struct ServerInfo ServerInfo;

using MessageType = MessageWrapper::MessageType;

BoardPlayer gameActorToPlayer(const GameActor& actor){
    if (actor == GameActor::WHITECLIENT)
        return BoardPlayer::White;
    else if (actor == GameActor::BLACKCLIENT)
        return BoardPlayer::Black;
    else
        return BoardPlayer::None;
}

void server_msg(const std::string& msg)
{
    static const std::string white = "\033[0m";
    static const std::string red = "\033[31m";

    std::cout << red << "[server]" << white << " " << msg << std::endl;
}
