#ifndef KATARENGA_SERVER_SERVER_UTILS_HPP
#define KATARENGA_SERVER_SERVER_UTILS_HPP

// Katarenga
#include <common/common_utils.hpp>

struct ServerCommon
{
    using GameId = Common::GameId;
};

void msg_server(const std::string& msg);
void msg_client(const std::string& msg);

static constexpr unsigned short MAX_CLIENT = 4;

#endif // KATARENGA_SERVER_SERVER_UTILS_HPP
