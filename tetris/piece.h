#pragma once

#include "def.h"

namespace tetris
{

class board;

class piece
{
public:
    int x;
    int y;
    int rotation;
    int type;
public:
    piece();
public:
    bool is_colliding(board& board);
public:
    bool move_right(board& board);
    bool move_left(board& board);
    bool move_cw(board& board, int& index);
    bool move_ccw(board& board, int& index);
    bool move_down(board& board);
    bool move_drop(board& board);
public:
    bool place(board& board);
public:
    int get_tspin(board& board, int index);
public:
    bool operator == (piece& other);
};

};