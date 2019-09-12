#pragma once
#include "Player.h"
#include <iostream>
#include <utility>
#include <vector>
#include <map>

using namespace std;

typedef vector<vector<Color>> Board;
typedef pair<int, int> Direction;
typedef map<int, vector<Direction>> Move;

class Game {
public:
	bool ChangePlayer(const Board&, Color);
	bool CheckAct(Move, int, int);
	int ExecAct(Board&, Move, int, int, Color);
	int UndoAct(Board&, Move, int, int, Color);
	Move GetAvaliableAct(const Board&, Color);
	int CheckEnd(const Board&);

	Player* players[2];
	Board board;
	Game();
	void Print();
	void Start();
private:
	int round;
	int currentPlayer;
	int winner;
	void init();
	void printWinner();
	void execAct(Move, int, int);
	int checkEnd();
	bool canMove(int, int);
	vector<Direction> getDirection(const Board&, int, int, Color);
};

