#pragma once

#include "def.h"

namespace tetris
{

class piece;

class board
{
public:
    int data[40][10];
public:
    board();
public:
    void reset();
public:
    int get(int x, int y);
    int get_clear();
    int get_clear(piece& piece);
    int get_tspin(piece& piece, int index);
public:
    int set(int x, int y, int type);
public:
    bool is_occupied(int x, int y);
    bool is_colliding(piece& piece);
    bool is_colliding(int x, int y, int type, int rotation);
    bool is_perfect();
public:
    bool clear();
};

};