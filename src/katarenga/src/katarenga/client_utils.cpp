#include "client_utils.hpp"

// Standard Library
#include <iostream>
#include <sstream>
#include <vector>

static const std::string BLACK         = std::string("\033[0;30m");
static const std::string RED           = std::string("\033[0;31m");
static const std::string GREEN         = std::string("\033[0;32m");
static const std::string YELLOW        = std::string("\033[0;33m");
static const std::string BLUE          = std::string("\033[0;34m");
static const std::string PURPLE        = std::string("\033[0;35m");
static const std::string CYAN          = std::string("\033[0;36m");
static const std::string WHITE         = std::string("\033[0;37m");
static const std::string DEFAULT_COLOR = std::string("\033[0m");

void msg_client(const std::string& msg)
{
    std::cout << GREEN << "[client]" << DEFAULT_COLOR << " " << msg << std::endl;
}

void warning_client(const std::string& msg)
{
    std::cout << CYAN << "[client]" << DEFAULT_COLOR << " " << msg << std::endl;
}

static std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> result;

   std::istringstream tokenStream(s);
   std::string token;

   while (std::getline(tokenStream, token, delimiter))
      result.push_back(token);

   return result;
}

Common::Move convert_to_move(const std::string& str_move)
{
    if(str_move.length() != 5)
        return {-1,-1,-1,-1};

    std::vector<std::string> splitted = split(str_move, ':');

    if(splitted.size() != 2)
        return {-1,-1,-1,-1};

    const std::string& from_cell = splitted[0];
    const std::string& to_cell = splitted[1];

    int from_line = int(from_cell[0]) - int('a');
    int from_row = int(from_cell[1]) - int('0');

    int to_line = int(to_cell[0]) - int('a');
    int to_row = int(to_cell[1]) - int('0');

    return {from_line, from_row, to_line, to_row};
}
