#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <zmqpp/zmqpp.hpp>
#include <string>

void graphics_function(zmqpp::context &zmq_context, const std::string render_binding_point);

#endif // GRAPHICS_HPP

