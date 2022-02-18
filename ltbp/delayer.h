#pragma once

#include <iostream>

class delayer
{
public:
    int counter = -1;
public:
    delayer();
public:
    void delay(int frame);
    void update();
public:
    bool is_delaying();
    bool is_just_end();
};