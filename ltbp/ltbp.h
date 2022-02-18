#pragma once

#include "player.h"

class ltbp : public olc::PixelGameEngine
{
public:
    player player[2];
public:
	ltbp();
public:
    bool OnUserCreate() override;
	bool OnUserUpdate(float dt) override;
	bool OnUserDestroy() override;
};