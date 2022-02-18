#include <iostream>

#define OLC_PGE_APPLICATION
#include "ltbp.h"

int main()
{
    srand(uint32_t(time(NULL)));

    ltbp game;
    if (game.Construct(1920, 1080, 1, 1, false, true, false)) {
        game.Start();
    }

    return 0;
};