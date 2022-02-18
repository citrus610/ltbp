#pragma once

#include "../tetris/debug.h"
#include "delayer.h"
#include "setting.h"
#include "statistic.h"

using namespace tetris;

enum
{
    MOVE_NULL,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_CW,
    MOVE_CCW,
    MOVE_DOWN
};

enum
{
    PLAYER_HUMAN,
    PLAYER_BOT
};

class player
{
public:
    int type;
public:
    board board;
    int current;
    int hold;
    int b2b;
    int combo;
    std::vector<int> queue;
    std::vector<int> bag;
    int bag_index;
public:
    piece piece;
    bool piece_locked;
    bool piece_holded;
    int piece_last_move;
    int piece_rotate_index;
    int piece_on_floor_counter;
    int piece_drop_counter;
public:
    player* enemy;
    int garbage_incomming;
public:
    bool gameover;
public:
    delayer delay_move;
    delayer delay_clear;
    setting rule;
    statistic stat;
public:
    olc::Sprite* sprite_board;
    olc::Sprite* sprite_block;
    olc::Sprite* sprite_ghost;
public:
    olc::Decal* decal_board;
    olc::Decal* decal_block;
    olc::Decal* decal_ghost;
public:
    player();
public:
    void set_bag(std::vector<int> bag);
    void set_enemy(player* other);
public:
    void reset();
    void init(int type, setting setting, std::vector<int> bag, player* enemy);
    void update(olc::PixelGameEngine* pge);
    void render(olc::PixelGameEngine* pge, int x, int y);
    void end();
public:
    void load_queue();
    void load_current();
public:
    void advance_state();
public:
    int calc_garbage();
public:
    void garbage_place();
    void garbage_send(int line);
public:
    void do_hold();
    void do_right();
    void do_left();
    void do_cw();
    void do_ccw();
    void do_down();
    void do_drop();
};