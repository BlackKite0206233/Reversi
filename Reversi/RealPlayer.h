#pragma once
#include "Game.h"

class RealPlayer : public Player {
public:
	pair<int, int> Act(const Board&) override;
};

