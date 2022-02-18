#pragma once

#include "piece.h"
#include "board.h"

namespace tetris
{

static void print_board(board& board)
{
    using namespace std;
    for (int y = 39; y >= 0; --y) {
        for (int x = 0; x < 10; ++x) {
            if (board.is_occupied(x, y)) {
                cout << "#";
            }
            else {
                cout << ".";
            }
        }
        cout << "\n";
    }
    cout << endl;
};

};