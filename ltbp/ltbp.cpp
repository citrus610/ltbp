#include "ltbp.h"

ltbp :: ltbp()
{
    this->player[0].reset();
    this->player[1].reset();

    this->sAppName = "Teatris";
};

bool ltbp :: OnUserCreate()
{
    srand(uint32_t(time(NULL)));

    std::vector<int> pregenerate_bag;
    pregenerate_bag.reserve(1000);
    while (true)
    {
        int init_bag[7] = {
            tetris::TYPE_I,
            tetris::TYPE_J,
            tetris::TYPE_L,
            tetris::TYPE_S,
            tetris::TYPE_Z,
            tetris::TYPE_T,
            tetris::TYPE_O
        };
        for (int i = 0; i < 7; ++i) {
            int value = init_bag[i];
            int swap = rand() % 7;
            init_bag[i] = init_bag[swap];
            init_bag[swap] = value;
        }
        for (int i = 0; i < 7; ++i) {
            pregenerate_bag.push_back(init_bag[i]);
        }
        if (pregenerate_bag.size() >= 1000) {
            break;
        }
    }

    this->player[0].init(PLAYER_HUMAN, setting(), pregenerate_bag, &this->player[1]);
    this->player[1].init(PLAYER_HUMAN, setting(), pregenerate_bag, &this->player[0]);

    return true;
};

bool ltbp :: OnUserUpdate(float dt)
{
    this->Clear(olc::BLACK);

    this->player[0].update(this);
    this->player[1].update(this);

    this->player[0].render(this, 280, 140);
    this->player[1].render(this, 280 + 960, 140);

    return true;
};

bool ltbp :: OnUserDestroy()
{
    this->player[0].end();
    this->player[1].end();
    return true;
};