#include "Board.hpp"

// TODO Implement rotations of the tiles
const char bb[8][4][4] = {{{'R', 'B', 'K', 'N'}, // 0
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

// Generates 4 different random values between 0 and 7.
void pickRand(int *a, int *b, int *c, int *d)
{
    srand (time(NULL));
    *a = rand()%8;
    do {
        *b = rand()%8;
    } while(*b == *a);
    do {
        *c = rand()%8;
    } while(*c == *a || *c == *b);
    do {
        *d = rand()%8;
    } while(*d == *a || *d == *b || *d == *c);
}

std::string generateBoardCellTypes()
{
    int a, b, c, d;
    pickRand(&a, &b, &c, &d);

    std::string s1 = "";
    std::string s2 = "";
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            s1 += bb[a][i][j];
            s2 += bb[c][i][j];
        }
        for(int j = 0; j < 4; ++j) {
            s1 += bb[b][i][j];
            s2 += bb[d][i][j];
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
