#pragma once
#include "Game.h"

class AIPlayer : public Player {
public:
	pair<int, int> Move(const Board&) override;
private:
	pair<int, int> negamax(Board&, Color, int);
	int alphabeta(Board&, Color, int, int, int);
	int score(Board&, Color);
};

