#pragma once
#include <utility>
#include <vector>
#include <map>

using namespace std;

enum class Color {
	Black = 1,
	White = -1,
	Empty = 0
};

typedef vector<vector<Color>> Board;
typedef pair<int, int> Direction;
typedef map<int, vector<Direction>> Move;

class Player {
public:
	Player();
	int num;
	Color color;
	void Init();
	virtual pair<int, int> Act(const Board&) = 0;
};

