#pragma once
#include <utility>

using namespace std;

enum class Color : char {
	Black = '@',
	White = 'O',
	Empty = ' '
};

class Player {
public:
	Player();
	int num;
	Color color;
	void Init();
	virtual pair<int, int> Move();
};

