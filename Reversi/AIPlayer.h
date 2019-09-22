#pragma once
#include "Reversi.h"

class AIPlayer : public Player {
public:
	AIPlayer(int);
	pair<int, int> Act(const Board&) override;
private:
	int depth;
	int negamax(Board&, Color);
	int alphabeta(Board&, Color, int, int, int);
	int score(Board&, Color);
};

