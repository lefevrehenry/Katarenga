#include "board_utils.hpp"

#include <common/board/Board.hpp>
#include <iostream>
#include <sstream>
#include <vector>

const char board_tiles[8][4][4] = {
                          {{'R', 'B', 'K', 'N'}, // 0
                           {'N', 'R', 'N', 'K'},
                           {'B', 'B', 'K', 'R'},
                           {'K', 'N', 'R', 'B'}},
                          {{'R', 'B', 'N', 'K'}, // 1
                           {'N', 'B', 'R', 'B'},
                           {'K', 'K', 'N', 'R'},
                           {'B', 'R', 'K', 'N'}},
                          {{'R', 'N', 'K', 'B'}, // 2
                           {'B', 'B', 'N', 'R'},
                           {'K', 'K', 'R', 'N'},
                           {'N', 'R', 'B', 'K'}},
                          {{'R', 'N', 'K', 'B'}, // 3
                           {'B', 'B', 'K', 'R'},
                           {'K', 'N', 'R', 'N'},
                           {'N', 'R', 'B', 'K'}},
                          {{'R', 'N', 'B', 'K'}, // 4
                           {'B', 'R', 'B', 'N'},
                           {'K', 'N', 'K', 'R'},
                           {'N', 'K', 'R', 'B'}},
                          {{'R', 'B', 'N', 'K'}, // 5
                           {'N', 'B', 'R', 'B'},
                           {'K', 'K', 'N', 'R'},
                           {'B', 'R', 'K', 'N'}},
                          {{'R', 'B', 'K', 'N'}, // 6
                           {'N', 'B', 'K', 'R'},
                           {'K', 'N', 'R', 'B'},
                           {'B', 'R', 'N', 'K'}},
                          {{'R', 'B', 'K', 'N'}, // 7
                           {'N', 'B', 'N', 'R'},
                           {'K', 'K', 'R', 'B'},
                           {'B', 'R', 'N', 'K'}}};

// Generates 4 different random values between 0 and 7
void randomPickTiles(int *a, int *b, int *c, int *d)
{
    srand (time(NULL));
    *a = rand() % 8;
    do {
        *b = rand() % 8;
    } while(*b == *a);
    do {
        *c = rand() % 8;
    } while(*c == *a || *c == *b);
    do {
        *d = rand() % 8;
    } while(*d == *a || *d == *b || *d == *c);
}

// Generates 4 random values between 0 and 3
void randomPickRotations(int *a, int *b, int *c, int *d)
{
    srand (time(NULL));
    *a = rand() % 4;
    *b = rand() % 4;
    *c = rand() % 4;
    *d = rand() % 4;
}

void generateRotation(char (&tab)[4][4], const char (&board_tile)[4][4], int rota)
{
    switch(rota)
    {
    case 0: // Normal rotation
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                tab[i][j] = board_tile[i][j];
            }
        }
        break;
    case 1: // 90° clockwise
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                tab[i][j] = board_tile[j][3 - i];
            }
        }
        break;
    case 2: // 180° clockwise
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                tab[i][j] = board_tile[3-i][3 - j];
            }
        }
        break;
    case 3: // 270° clockwise
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                tab[i][j] = board_tile[3 - j][i];
            }
        }
        break;
    }
}


std::string generateBoardCellTypes()
{
    int tile_top_left, tile_top_right, tile_bot_left, tile_bot_right;
    randomPickTiles(&tile_top_left, &tile_top_right, &tile_bot_left, &tile_bot_right);

    int rota_top_left, rota_top_right, rota_bot_left, rota_bot_right;
    randomPickRotations(&rota_top_left, &rota_top_right, &rota_bot_left, &rota_bot_right);

    char top_left[4][4];
    char top_right[4][4];
    char bot_left[4][4];
    char bot_right[4][4];

    generateRotation(top_left, board_tiles[tile_top_left], rota_top_left);
    generateRotation(top_right, board_tiles[tile_top_right], rota_top_right);
    generateRotation(bot_left, board_tiles[tile_bot_left], rota_bot_left);
    generateRotation(bot_right, board_tiles[tile_bot_right], rota_bot_right);

    std::string s1 = "";
    std::string s2 = "";
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            s1 += top_left[i][j];
            s2 += bot_left[i][j];
        }
        for(int j = 0; j < 4; ++j) {
            s1 += top_right[i][j];
            s2 += bot_right[i][j];
        }
    }

    std::string boardString = s1 + s2;
    return boardString;
}

std::string format_board(const std::string& board_configuration)
{
    std::string s = "";

    s += "\n";
    s += "    1  2  3  4  5  6  7  8   ";
    s += "\n";
    s += "             White           ";
    s += "\n";
    s += "  X                         X";
    s += "\n";

    std::string board = board_configuration.substr(0,128);

    for (int j = 0; j < 8; ++j) {
        s += std::to_string(j+1) + "  ";
        for (int i = 0; i < 8; ++i) {
            size_t index = (j * 8) + i;
            std::string c = board.substr(2*index,2);
            s += " " + c;
        }
        s += "\n";
    }

    s += "  X                         X";
    s += "\n";
    s += "             Black           ";

    return s;
}

BoardPlayer otherPlayer(const BoardPlayer &player)
{
    if (player == BoardPlayer::White)
        return BoardPlayer::Black;
    else if (player == BoardPlayer::Black)
        return BoardPlayer::White;
   else
        return BoardPlayer::None;
}

std::string to_string(const BoardPlayer &player)
{
    std::string s;
    if (player == BoardPlayer::White)
        s = "White";
    else if (player == BoardPlayer::Black)
        s = "Black";
    else
        s = "None";
    return s;
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

static bool is_valid_cell(const std::string& str_cell)
{
    if(str_cell.size() != 2)
        return false;

    return true;
}

bool is_valid_move(const Common::Move& move)
{
    int from_line = std::get<0>(move);
    int from_row = std::get<1>(move);

    int to_line = std::get<2>(move);
    int to_row = std::get<3>(move);

    return (from_line >= 0 && from_line <= 8 &&
            from_row >= 0 && from_row <= 8 &&
            to_line >= 0 && to_line <= 8 &&
            to_row >= 0 && to_row <= 8);
}

Common::Move convert_to_move(const std::string& str_move)
{
    Common::Move move;

    if(str_move.length() != 5)
        return move;

    std::vector<std::string> splitted = split(str_move, ':');

    if(splitted.size() != 2)
        return move;

    const std::string& from_cell = splitted[0];
    const std::string& to_cell = splitted[1];

    int from_line = int(from_cell[0]) - int('a');
    int from_row = int(from_cell[1]) - int('0');

    int to_line = int(to_cell[0]) - int('a');
    int to_row = int(to_cell[1]) - int('0');

    return Common::Move({from_line, from_row, to_line, to_row});
}
