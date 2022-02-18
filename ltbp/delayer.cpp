#include "delayer.h"

delayer :: delayer()
{
    this->counter = -1;
};

void delayer :: delay(int frame)
{
    this->counter += frame + 1;
};

void delayer :: update()
{
    this->counter = this->counter - 1;
    if (this->counter < 0) {
        this->counter = -1;
    }
};

bool delayer :: is_delaying()
{
    return this->counter > 0;
};

bool delayer :: is_just_end()
{
    return this->counter == 0;
};