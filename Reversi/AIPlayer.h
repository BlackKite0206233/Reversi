#pragma once
#include "Game.h"

class AIPlayer : public Player {
public:
	pair<int, int> Move() override;
};

