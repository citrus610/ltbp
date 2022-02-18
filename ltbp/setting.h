#pragma once

#include "../lib/pge.h"

constexpr int COMBO_TABLE_SIZE = 12;

struct setting_delay
{
    int das = 9;
    int arr = 1;
    int softdrop = 2;
    int spawn = 7;
    int on_floor_lock = 30;
    int fall = 30;
    int clear_1 = 35;
    int clear_2 = 40;
    int clear_3 = 40;
    int clear_4 = 45;
    int clear_perfect = 1;
};

struct setting_attack
{
    int clear_1 = 0;
    int clear_2 = 1;
    int clear_3 = 2;
    int clear_4 = 4;
    int tspin_1 = 2;
    int tspin_2 = 4;
    int tspin_3 = 6;
    int tspin_mini_1 = 1;
    int tspin_mini_2 = 2;
    int perfect = 10;
    int b2b_bonus = 1;
    int combo[COMBO_TABLE_SIZE] = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4, 5 };
};

struct setting_garbage
{
    int messiness_percent = 30;
    bool blocking = false;
    bool countering = true;
};

struct setting_player
{
    int preview = 5;
};

struct setting_input
{
    int right = olc::RIGHT;
    int left = olc::LEFT;
    int cw = olc::UP;
    int ccw = olc::Z;
    int down = olc::DOWN;
    int drop = olc::SPACE;
    int hold = olc::C;
};

struct setting_color
{
    olc::Pixel map[tetris::TYPE_COUNT] = {
        olc::Pixel(0, 152, 188, 255),
        olc::Pixel(38, 77, 133, 255),
        olc::Pixel(218, 113, 31, 255),
        olc::Pixel(228, 178, 3, 255),
        olc::Pixel(0, 121, 106, 255),
        olc::Pixel(93, 63, 136, 255),
        olc::Pixel(193, 21, 79, 255),
        olc::DARK_GREY
    };
};

struct setting
{
    setting_delay delay = setting_delay();
    setting_attack attack = setting_attack();
    setting_garbage garbage = setting_garbage();
    setting_player player = setting_player();
    setting_input input = setting_input();
    setting_color color = setting_color();
};