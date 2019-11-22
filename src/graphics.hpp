#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

void graphics_function(int this_player, int socket_port, bool verbose);

void standalone_graphics_function(int socket_port, bool verbose);

std::string askNextMoveText(int current_player);

#endif // GRAPHICS_HPP

