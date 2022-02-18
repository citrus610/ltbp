#pragma once

#include <cmath>
#include <cstdint>
#include <string>
#include <iostream>
#include <streambuf>
#include <sstream>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <fstream>
#include <map>
#include <functional>
#include <algorithm>
#include <array>
#include <cstring>

namespace tetris
{

enum
{
    TYPE_I,
    TYPE_J,
    TYPE_L,
    TYPE_O,
    TYPE_S,
    TYPE_T,
    TYPE_Z,
    TYPE_GARBAGE,
    TYPE_NULL,
    TYPE_COUNT = 8
};

enum
{
    ROTATION_UP,
    ROTATION_RIGHT,
    ROTATION_DOWN,
    ROTATION_LEFT
};

enum
{
    TSPIN_NULL,
    TSPIN_MINI,
    TSPIN_FULL
};

enum
{
    LOCK_NULL,
    LOCK_CLEAR_1,
    LOCK_CLEAR_2,
    LOCK_CLEAR_3,
    LOCK_CLEAR_4,
    LOCK_TSPIN_1,
    LOCK_TSPIN_2,
    LOCK_TSPIN_3,
    LOCK_TSPIN_MINI_1,
    LOCK_TSPIN_MINI_2,
    LOCK_PERFECT
};

constexpr int PIECE_LUT[7][4][4][2] =
{
    {
        {{ -1, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 }},
        {{ 0, 1 }, { 0, 0 }, { 0, -1 }, { 0, -2 }},
        {{ 1, 0 }, { 0, 0 }, { -1, 0 }, { -2, 0 }},
        {{ 0, -1 }, { 0, 0 }, { 0, 1 }, { 0, 2 }}
    },
    {
        {{ -1, 0 }, { 0, 0 }, { 1, 0 }, { -1, 1 }},
        {{ 0, 1 }, { 0, 0 }, { 0, -1 }, { 1, 1 }},
        {{ 1, 0 }, { 0, 0 }, { -1, 0 }, { 1, -1 }},
        {{ 0, -1 }, { 0, 0 }, { 0, 1 }, { -1, -1 }}
    },
    {
        {{ -1, 0 }, { 0, 0 }, { 1, 0 }, { 1, 1 }},
        {{ 0, 1 }, { 0, 0 }, { 0, -1 }, { 1, -1 }},
        {{ 1, 0 }, { 0, 0 }, { -1, 0 }, { -1, -1 }},
        {{ 0, -1 }, { 0, 0 }, { 0, 1 }, { -1, 1 }}
    },
    {
        {{ 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 }}, 
        {{ 0, 0 }, { 0, -1 }, { 1, 0 }, { 1, -1 }},
        {{ 0, 0 }, { -1, 0 }, { 0, -1 }, { -1, -1 }},
        {{ 0, 0 }, { 0, 1 }, { -1, 0 }, { -1, 1 }}
    },
    {
        {{ -1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 1 }},
        {{ 0, 1 }, { 0, 0 }, { 1, 0 }, { 1, -1 }},
        {{ 1, 0 }, { 0, 0 }, { 0, -1 }, { -1, -1 }},
        {{ 0, -1 }, { 0, 0 }, { -1, 0 }, { -1, 1 }}
    },
    {
        {{ -1, 0 }, { 0, 0 }, { 1, 0 }, { 0, 1 }},
        {{ 0, 1 }, { 0, 0 }, { 0, -1 }, { 1, 0 }},
        {{ 1, 0 }, { 0, 0 }, { -1, 0 }, { 0, -1 }},
        {{ 0, -1 }, { 0, 0 }, { 0, 1 }, { -1, 0 }}
    },
    {
        {{ -1, 1 }, { 0, 1 }, { 0, 0 }, { 1, 0 }},
        {{ 1, 1 }, { 1, 0 }, { 0, 0 }, { 0, -1 }},
        {{ 1, -1 }, { 0, -1 }, { 0, 0 }, { -1, 0 }},
        {{ -1, -1 }, { -1, 0 }, { 0, 0 }, { 0, 1 }}
    }
};

constexpr int SRS_LUT[2][4][5][2] =
{
    {
        {{ 0, 0 }, { -1, 0 }, { 2, 0 }, { -1, 0 }, { 2, 0 }},
        {{ -1, 0 }, { 0, 0 }, { 0, 0 }, { 0, 1 }, { 0, -2 }},
        {{ -1, 1 }, { 1, 1 }, { -2, 1 }, { 1, 0 }, { -2, 0 }},
        {{ 0, 1 }, { 0, 1 }, { 0, 1 }, { 0, -1 }, { 0, 2 }},
    },
    {
        {{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }},
        {{ 0, 0 }, { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 }},
        {{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }},
        {{ 0, 0 }, { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 }},
    }
};

};