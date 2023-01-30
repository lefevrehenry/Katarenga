#ifndef KATARENGA_CLIENT_CLIENT_UTILS_HPP
#define KATARENGA_CLIENT_CLIENT_UTILS_HPP

// Katarenga
#include <common/common_utils.hpp>

// Standard Library
#include <string>

void msg_client(const std::string& msg);
void warning_client(const std::string& msg);

Common::Move convert_to_move(const std::string& move);

#endif // KATARENGA_CLIENT_CLIENT_UTILS_HPP
