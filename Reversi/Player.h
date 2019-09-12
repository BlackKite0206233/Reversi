#pragma once
#include <utility>

using namespace std;

enum class Color {
	Black = 1,
	White = -1,
	Empty = 0
};

class Player {
public:
	Player();
	int num;
	Color color;
	void Init();
	virtual pair<int, int> Move(const Board&);
};

