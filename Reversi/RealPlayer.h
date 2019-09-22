#pragma once
#include "Reversi.h"

class RealPlayer : public Player {
public:
	RealPlayer() {
		playerType = PlayerType::Real;
	}
	pair<int, int> Act(const Board&) {
		return pair<int, int>(0, 0);
	}
};

